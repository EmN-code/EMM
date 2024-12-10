
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/ipv6/addr.h>
#include <string.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void publish_state(MQTTClient *client) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(client, TOPIC, &message);
}

int main(void) {
    Network network;
    MQTTClient client;

    // Initialize the network
    sock_tcp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    sock_tcp_create(&network.sock, &local, &remote, 0);

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, INTERVAL, NULL, 0, NULL, 0);

    // Connect to the MQTT broker
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_MQTT_CLIENT";
    data.keepAliveInterval = INTERVAL / 1000000U; // Convert to seconds

    if (MQTTConnect(&client, &data) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state(&client);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
