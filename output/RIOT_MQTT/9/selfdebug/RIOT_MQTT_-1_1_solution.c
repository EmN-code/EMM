#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "mqtt.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define STATE_TOPIC "device/state"
#define STATE_MSG_WORK "work"
#define STATE_MSG_NOT_WORK "not work"
#define WAKEUP_INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

static mqtt_client_t client;

void publish_state(bool is_working) {
    MQTTMessage message;
    memset(&message, 0, sizeof(message));
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)((is_working) ? STATE_MSG_WORK : STATE_MSG_NOT_WORK);
    message.payloadlen = strlen((char *) message.payload);
    MQTTPublish(&client, STATE_TOPIC, &message);
}

int main(void) {
    Network n;
    int rc = 0;

    network_init(&n);
    mqtt_client_init(&client, &n, 3000, NULL, 0, NULL, 0);

    printf("Connecting to MQTT broker at %s:%d...\n", SERVER_ADDR, SERVER_PORT);
    rc = NetworkConnect(&n, SERVER_ADDR, SERVER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    rc = MQTTConnect(&client);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    printf("Connected to the broker\n");

    while (1) {
        publish_state(true);  // Publish 'work'
        xtimer_usleep(WAKEUP_INTERVAL);
        publish_state(false);  // Publish 'not work'
        xtimer_usleep(WAKEUP_INTERVAL);
    }

    return 0;
}

void network_init(Network *network) {
    NetworkDisconnect(network);  // Ensure no prior connections exist
}