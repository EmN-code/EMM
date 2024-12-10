
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_CLEAN_SESSION 1
#define MQTT_PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void mqtt_connect(void);
static void mqtt_publish(void);

static sock_tcp_t mqtt_sock;
static struct Network mqtt_network;
static MQTTClient mqtt_client;

int main(void)
{
    puts("Starting MQTT client...");

    // Initialize the network structure
    mqtt_network.mqttread = sock_tcp_read;
    mqtt_network.mqttwrite = sock_tcp_write;

    // Connect to the MQTT broker
    mqtt_connect();

    // Initialize the MQTT client
    MQTTClientInit(&mqtt_client, &mqtt_network, MQTT_KEEPALIVE, NULL, 0, NULL, 0);

    // Set the client ID
    MQTTSetClientID(&mqtt_client, MQTT_CLIENT_ID);

    // Connect to the broker
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID.cstring = MQTT_CLIENT_ID;
    data.keepAliveInterval = MQTT_KEEPALIVE;
    data.cleansession = MQTT_CLEAN_SESSION;

    if (MQTTConnect(&mqtt_client, &data) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    puts("Connected to MQTT broker");

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish();
        xtimer_usleep(MQTT_PUBLISH_INTERVAL);
    }

    return 0;
}

static void mqtt_connect(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };

    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&mqtt_sock, &local, &remote, 0) < 0) {
        puts("Failed to create TCP socket");
        return;
    }

    mqtt_network.sock = mqtt_sock;
}

static void mqtt_publish(void)
{
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.id = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    if (MQTTPublish(&mqtt_client, MQTT_TOPIC, &message) != 0) {
        puts("Failed to publish message");
    } else {
        puts("Message published");
    }
}
