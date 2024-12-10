#include "ztimer.h"
#include "xtimer.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define EMCUTE_PORT     (1883U)
#define TOPIC_MAXLEN    64U

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t topic;

static const char *mqtt_topic = "state";
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, "riot-client");
    return NULL;
}

int main(void) {
    puts("RIOT MQTT-SN example\n");

    gw.port = EMCUTE_PORT;
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect\n");
        return 1;
    }

    topic.name = mqtt_topic;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return 1;
    }

    while (1) {
        char *message = "work";
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish\n");
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}