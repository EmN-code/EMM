#include <stdio.h>
#include <net/net.h>
#include <xtimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static void *stack[THREAD_STACKSIZE_MAIN];

int main(void) {
    MQTTClient client;
    Network network;
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    int rc = 0;

    printf("Connecting to MQTT Broker at %s:%d...\n", BROKER_IP, BROKER_PORT);

    NetworkInit(&network);
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    options.MQTTVersion = 3;
    options.clientID.cstring = "esp32_client";

    rc = MQTTConnect(&client, &options);
    if (rc != SUCCESS) {
        printf("MQTT connect failed with code %d\n", rc);
        return rc;
    }

    printf("MQTT connected\n");

    while (1) {
        MQTTMessage message;
        char *payload = "state: work";

        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void *)payload;
        message.payloadlen = strlen(payload);

        MQTTPublish(&client, "state_topic", &message);

        printf("Published: %s\n", payload);
        xtimer_usleep(PUB_INTERVAL);
    }

    return 0;
}