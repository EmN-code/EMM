#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT         (1883U)
#define EMCUTE_ID           (10U)
#define MQTT_TOPIC_NAME     "work_status"
#define STATE_MESSAGE       "work"
#define SLEEP_INTERVAL      (5U)

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, emcute_stack, sizeof(emcute_stack), EMCUTE_PRIO);
    return NULL;
}

int main(void) {
    thread_create(emcute_stack, sizeof(emcute_stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    while (1) {
        int res = emcute_con(&gw, true, NULL);
        if (res == EMCUTE_OK) {
            printf("Connected to gateway\n");
            break;
        } else {
            printf("Connection failed! Retrying...\n");
            xtimer_sleep(1);
        }
    }

    strcpy(topic.name, MQTT_TOPIC_NAME);
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    while (1) {
        int rc = emcute_pub(&topic, STATE_MESSAGE, strlen(STATE_MESSAGE), EMCUTE_QOS_0);
        if (rc != EMCUTE_OK) {
            printf("Publish failed with code %d
", rc);
        } else {
            printf("Published '%s' to topic '%s'
", STATE_MESSAGE, topic.name);
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}