#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP     "47.102.103.1"
#define MQTT_BROKER_PORT   1883
#define PUBLISH_INTERVAL   (5 * 1000000U) // 5 seconds
#define MQTT_TOPIC         "work/state"

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

int mqtt_publish_state(const char *state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    return MQTTPublish(&client, MQTT_TOPIC, &message);
}

int main(void) {
    char* state = "work";
    int rc;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network\n");
        return 1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 2;
    }

    while(1) {
        rc = mqtt_publish_state(state);
        if (rc != 0) {
            printf("Failed to publish message\n");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}