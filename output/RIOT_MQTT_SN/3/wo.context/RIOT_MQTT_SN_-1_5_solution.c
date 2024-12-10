#include <thread.h>
#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <emcute.h>

#define EMCUTE_PORT     1883
#define EMCUTE_ID       "device123"
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define STATE_TOPIC     "state"
#define MQTT_INTERVAL   (5U * US_PER_SEC) // 5 seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

void *publish_thread(void *arg) {
    (void)arg;
    while (1) {
        // Toggle message to publish
        static bool work = false;
        const char *message = work ? "work" : "not work";
        work = !work;

        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        } else {
            printf("Published: %s\n", message);
        }

        xtimer_sleep(MQTT_INTERVAL / US_PER_SEC);
    }
    return NULL;
}

int main(void) {
    // Create MQTT-SN publisher thread
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, publish_thread, NULL, "publish_thread");

    // Emcute initialization
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to the broker\n");
        return 1;
    }

    // Register a topic
    topic.name = STATE_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    printf("Connected and registered to broker at %s:%d\n", "47.102.103.1", EMCUTE_PORT);
    return 0;
}