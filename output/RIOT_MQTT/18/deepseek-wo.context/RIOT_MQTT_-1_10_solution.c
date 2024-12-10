
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void mqtt_connect(MQTTClient *client, Network *network)
{
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "riot_client";
    data.keepAliveInterval = 60;
    data.cleansession = 1;

    int rc = MQTTConnect(client, &data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
    } else {
        printf("Connected to MQTT broker\n");
    }
}

static void mqtt_publish(MQTTClient *client, const char *topic, const char *payload, int payload_len)
{
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)payload;
    message.payloadlen = payload_len;

    int rc = MQTTPublish(client, topic, &message);
    if (rc != 0) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Message published\n");
    }
}

int main(void)
{
    Network network;
    MQTTClient client;
    int rc;

    // Initialize network
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    sock_tcp_t sock;
    sock_tcp_create(&sock, &local, &remote, SOCK_FLAGS_REUSE_EP);

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);
    network.sock = sock;
    network.mqttread = sock_tcp_read;
    network.mqttwrite = sock_tcp_write;

    // Connect to MQTT broker
    mqtt_connect(&client, &network);

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish(&client, TOPIC, PAYLOAD, PAYLOAD_LEN);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
