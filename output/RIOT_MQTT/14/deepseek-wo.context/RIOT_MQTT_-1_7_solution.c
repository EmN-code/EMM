
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_YIELD_POLLING_MS 30

static MQTTClient mqtt_client;
static Network mqtt_network;
static unsigned char sendbuf[256];
static unsigned char readbuf[256];

void mqtt_connect(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_esp32";

    int rc = MQTTConnect(&mqtt_client, &options);
    if (rc != 0) {
        printf("MQTT connect failed with error code %d\n", rc);
    } else {
        printf("MQTT connected successfully\n");
    }
}

void mqtt_publish(const char *topic, const char *payload) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);

    int rc = MQTTPublish(&mqtt_client, topic, &message);
    if (rc != 0) {
        printf("MQTT publish failed with error code %d\n", rc);
    } else {
        printf("MQTT published successfully: %s\n", payload);
    }
}

int main(void) {
    printf("Starting MQTT client on ESP32\n");

    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    sock_tcp_t sock;
    sock_tcp_create(&sock, &local, &remote, SOCK_TCP_FLAGS_REUSE_EP);

    mqtt_network.sock = sock;
    mqtt_network.mqttread = sock_tcp_read;
    mqtt_network.mqttwrite = sock_tcp_write;

    MQTTClientInit(&mqtt_client, &mqtt_network, MQTT_YIELD_POLLING_MS, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    mqtt_connect();

    while (1) {
        mqtt_publish(MQTT_TOPIC, MQTT_PAYLOAD);
        xtimer_sleep(5);
    }

    return 0;
}
