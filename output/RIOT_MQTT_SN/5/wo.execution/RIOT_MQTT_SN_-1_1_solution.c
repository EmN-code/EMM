#include "ztimer.h"
#include "stdio.h"
#include "net/emcute.h"
#include "string.h"
#include "xtimer.h"

#define MQTT_SN_BROKER_IP   "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define MQTT_SN_TOPIC       "state/status"
#define PUBLISH_INTERVAL    (5U) // in seconds

static emcute_topic_t topic;
static char *status = "work"; // initial state

static void emcute_on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Published to topic '%s' [%i] bytes\n", topic->name, (int)len);
}

int main(void) {
    // Initialize the emcute client
    emcute_sub_t subscription;
    char *topics[] = {MQTT_SN_TOPIC};
    topic.name = topics[0];
    topic.id = 0;
    
    printf("Connecting to MQTT-SN broker %s:%d\n", MQTT_SN_BROKER_IP, MQTT_SN_BROKER_PORT);

    // Start the emcute client
    if (emcute_connect((sock_ip_t *)MQTT_SN_BROKER_IP, MQTT_SN_BROKER_PORT) != EMCUTE_OK) {
        printf("Error: Unable to connect to the broker\n");
        return 1;
    }

    // Prepare the subscription
    subscription.cb = emcute_on_pub;
    subscription.topic.name = MQTT_SN_TOPIC;
    subscription.topic.id = 0;

    // Publish the initial state
    if (emcute_pub(&topic, status, strlen(status), EMCUTE_QOS_2) != EMCUTE_OK) {
        printf("Error: Unable to publish\n");
        return 1;
    }

    // Enter the main loop
    while (1) {
        // Toggle status between "work" and "not work"
        status = (strcmp(status, "work") == 0) ? "not work" : "work";

        // Publish new status
        if (emcute_pub(&topic, status, strlen(status), EMCUTE_QOS_2) != EMCUTE_OK) {
            printf("Error: Unable to publish\n");
        }

        // Wait for the next publishing
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL * 1000U);
    }

    return 0;
}