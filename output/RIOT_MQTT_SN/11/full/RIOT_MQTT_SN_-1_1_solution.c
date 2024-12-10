#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <thread.h>
#include <saul_reg.h>
#include <net/emcute.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define PUBLISH_INTERVAL 5

static emcute_topic_t topic;

int main(void) {
    emcute_topic_t topic;
    emcute_sub_t sub;
    sub.topic.id = 2;
    network n;
    thread_create(NULL, 0, 0, 0, worker_thread, NULL, "worker");
    printf("MQTT client started\n");

    while (1) {
        int rc = NetworkConnect(&n, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Network connection successful\n");

        MQTTClient client;
        MQTTStartTask(&client);
        MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
        rc = MQTTConnect(&client, &options);

        if (rc != 0) {
            printf("MQTT connection failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        }

        topic.name = MQTT_TOPIC;
        topic.id = EMCUTE_ID_ANY;

        while (1) {
            const char *state = "work"; // This could be dynamic
            MQTTMessage message;
            message.qos = QOS0;
            message.retained = 0;
            message.payload = (void *)state;
            message.payloadlen = strlen(state);

            rc = MQTTPublish(&client, topic.name, &message);

            if (rc != 0) {
                printf("MQTT publish failed, retrying...\n");
                xtimer_sleep(1);
                break;
            }
            printf("Published state: %s\n", state);

            xtimer_sleep(PUBLISH_INTERVAL);
        }
    }
    return 0;
}

void *worker_thread(void *arg) {
    while (1) {
        mutex_lock_cancelable(NULL);  // Just to simulate low power mode
        xtimer_sleep(1);
        mutex_unlock(NULL);
    }
    return NULL;
}