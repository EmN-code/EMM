
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_YIELD_POLLING_MS (30)

static void mqtt_connect(void);
static void mqtt_publish(void);

static sock_tcp_t mqtt_sock;
static uint8_t mqtt_sendbuf[128];
static uint8_t mqtt_readbuf[128];

int main(void)
{
    puts("MQTT client starting");

    /* Initialize the TCP socket */
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&mqtt_sock, &local, &remote, 0) < 0) {
        puts("Error creating TCP socket");
        return 1;
    }

    /* Connect to the MQTT broker */
    mqtt_connect();

    /* Publish state every 5 seconds */
    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}

static void mqtt_connect(void)
{
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    options.keepAliveInterval = 60;
    options.cleansession = 1;

    MQTTClient client;
    Network network;
    network.mqttread = sock_tcp_read;
    network.mqttwrite = sock_tcp_write;
    network.sock = mqtt_sock;

    MQTTClientInit(&client, &network, MQTT_YIELD_POLLING_MS, mqtt_sendbuf, sizeof(mqtt_sendbuf), mqtt_readbuf, sizeof(mqtt_readbuf));

    int rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        puts("Error connecting to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

static void mqtt_publish(void)
{
    MQTTClient client;
    Network network;
    network.mqttread = sock_tcp_read;
    network.mqttwrite = sock_tcp_write;
    network.sock = mqtt_sock;

    MQTTClientInit(&client, &network, MQTT_YIELD_POLLING_MS, mqtt_sendbuf, sizeof(mqtt_sendbuf), mqtt_readbuf, sizeof(mqtt_readbuf));

    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.id = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    int rc = MQTTPublish(&client, MQTT_TOPIC, &message);
    if (rc != 0) {
        puts("Error publishing message");
        return;
    }

    puts("Message published");
}
