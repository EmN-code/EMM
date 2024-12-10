#include "net/sock/udp.h"
#include "kernel_types.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

static msg_t queue[4];
static MQTTClient client;
static Network network;
static unsigned char sendbuf[80], readbuf[80];

static void connect_broker(void) {
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    int rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Failed to connect to broker, return code %d\n", rc);
    } else {
        printf("Connected to broker\n");
    }
}

static void publish_state(void) {
    MQTTMessage message;
    char *payload = "work";
    message.qos = QOS0;
    message.retained = 0;
    message.payload = payload;
    message.payloadlen = strlen(payload);

    int rc = MQTTPublish(&client, "esp32/state", &message);
    if (rc != SUCCESS) {
        printf("Failed to publish, return code %d\n", rc);
    } else {
        printf("Message published\n");
    }
}

int main(void) {
    msg_init_queue(queue, 4);
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    printf("Connecting to broker...\n");
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    connect_broker();

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    NetworkDisconnect(&network);
    return 0;
}