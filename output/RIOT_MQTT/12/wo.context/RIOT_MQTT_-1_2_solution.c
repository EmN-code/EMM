#include "xtimer.h"
#include "timex.h"
#include "net/af.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_mqtt_client"
#define MQTT_TOPIC "state"
#define MQTT_PUBLISH_INTERVAL (5 * US_PER_SEC)  // 5 seconds
#define MQTT_SEND_BUF_SIZE 512
#define MQTT_READ_BUF_SIZE 512

static char sendbuf[MQTT_SEND_BUF_SIZE];
static char readbuf[MQTT_READ_BUF_SIZE];

void *mqtt_thread(void *arg) {
    (void)arg;
    MQTTClient client;
    Network network;
    MQTTMessage message;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    MQTTClientInit(&client, &network, 1000, sendbuf, MQTT_SEND_BUF_SIZE, readbuf, MQTT_READ_BUF_SIZE);

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    printf("Connected to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    while (1) {
        char *state_message = "work";
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)state_message;
        message.payloadlen = strlen(state_message);

        if (MQTTPublish(&client, MQTT_TOPIC, &message) != SUCCESS) {
            printf("Failed to publish message\n");
        } else {
            printf("Published message: %s\n", state_message);
        }

        xtimer_sleep(MQTT_PUBLISH_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    mqtt_thread(NULL);
    return 0;
}