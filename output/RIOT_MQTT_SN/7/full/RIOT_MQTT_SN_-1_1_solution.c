#include <xtimer.h>
#include <thread.h>
#include "net/emcute.h"
#include "msg.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (1U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[4];

#define MQTT_BROKER_IP      "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define PUB_INTERVAL        (5U)  // 5 seconds

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topic[TOPIC_MAXLEN]

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, ARRAY_SIZE(queue));

    emcute_run(EMCUTE_PORT, stack, sizeof(stack), EMCUTE_PRIO);
    return NULL;  // This function should never return
}

int main(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    int rc;
    emcute_topic_t t;
    t.name = topic;

    /* Network connection setup */
    while (1) {
        rc = emcute_con(MQTT_BROKER_IP, MQTT_BROKER_PORT, false);
        if (rc != EMCUTE_OK) {
            printf("Network connect failed, retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Network connected successfully.\n");
            break;
        }
    }

    /* Prepare topic */
    strncpy(t.name, "state", sizeof(t.name));
    t.id = 0;  // Let the broker assign the topic ID.

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        /* Create publish message */
        const char *status = "work or not work";
        printf("Publishing status: %s\n", status);

        /* Publish the message */
        rc = emcute_pub(&t, status, strlen(status), EMCUTE_QOS_0);
        if (rc != EMCUTE_OK) {
            printf("Publish failed.\n");
        }

        xtimer_periodic_wakeup(&last_wakeup, PUB_INTERVAL * 1000000);
    }

    return 0;
}