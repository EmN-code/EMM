#include "xtimer.h"
#include "stdio.h"
#include "net/af.h"
#include "net/sock/udp.h"
#include "mqtt/MQTTClient.h"
#include "net/emcute.h"

#define BROKER_IP   "47.102.103.1"
#define BROKER_PORT 1883
#define INTERVAL    (5U * 1000000U)  // 5 seconds

static void publish_state(MQTTClient *client) {
    MQTTMessage message;
    char payload[20];

    snprintf(payload, sizeof(payload), "work");
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);

    MQTTClient_publish(client, "device/state", &message);
    printf("Published: %s\n", payload);
}

int main(void) {
    printf("MQTT example application
");

    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv6);

    MQTTClient client;
    Network network;
    MQTTClientInit(&client, &network, 1000, malloc(100), 100, malloc(100), 100);

    NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "esp32client";

    MQTTConnect(&client, &options);
    printf("Connected to broker
");

    while (1) {
        publish_state(&client);
        xtimer_sleep(5);
    }

    return 0;
}