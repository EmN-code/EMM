
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_NOT_WORK "not work"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_CLEAN_SESSION 1
#define MQTT_TIMEOUT 1000
#define MQTT_BUFFER_SIZE 1024
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_YIELD_POLLING_MS 30

static MQTTClient mqtt_client;
static Network mqtt_network;
static unsigned char sendbuf[MQTT_BUFFER_SIZE];
static unsigned char readbuf[MQTT_BUFFER_SIZE];

void mqtt_connect(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = MQTT_CLEAN_SESSION;
    options.MQTTVersion = 3;

    int rc = MQTTConnect(&mqtt_client, &options);
    if (rc != 0) {
        printf("MQTT connect failed, return code: %d\n", rc);
    } else {
        printf("MQTT connected successfully\n");
    }
}

void mqtt_publish(const char *message) {
    MQTTMessage mqtt_message;
    mqtt_message.qos = MQTT_QOS;
    mqtt_message.retained = MQTT_RETAIN;
    mqtt_message.payload = (void *)message;
    mqtt_message.payloadlen = strlen(message);

    int rc = MQTTPublish(&mqtt_client, MQTT_TOPIC, &mqtt_message);
    if (rc != 0) {
        printf("MQTT publish failed, return code: %d\n", rc);
    } else {
        printf("MQTT message published: %s\n", message);
    }
}

void mqtt_yield(void) {
    int rc = MQTTYield(&mqtt_client, MQTT_YIELD_POLLING_MS);
    if (rc != 0) {
        printf("MQTT yield failed, return code: %d\n", rc);
    }
}

int main(void) {
    printf("Starting MQTT client on ESP32\n");

    sock_tcp_ep_t broker_addr = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.addr.ipv4);

    sock_tcp_t sock;
    int rc = sock_tcp_connect(&sock, &broker_addr, SOCK_FLAGS_REUSE_EP);
    if (rc < 0) {
        printf("Failed to connect to MQTT broker, return code: %d\n", rc);
        return 1;
    }

    NetworkInit(&mqtt_network);
    mqtt_network.sock = sock;
    mqtt_network.mqttread = sock_tcp_read;
    mqtt_network.mqttwrite = sock_tcp_write;

    MQTTClientInit(&mqtt_client, &mqtt_network, MQTT_TIMEOUT, sendbuf, MQTT_BUFFER_SIZE, readbuf, MQTT_BUFFER_SIZE);

    mqtt_connect();

    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        mqtt_yield();

        if (xtimer_usec_from_ticks(xtimer_diff(xtimer_now(), last_wakeup)) >= 5000000) {
            last_wakeup = xtimer_now();
            mqtt_publish(MQTT_MESSAGE);
        }

        xtimer_usleep(100000);
    }

    return 0;
}
