#include <thread.h>
#include <net/emcute.h>
#include <event/thread.h>
#include <stdio.h>
#include <xtimer.h>
#include <net/af.h>

#define BROKER_IP         "47.102.103.1"
#define BROKER_PORT       1883
#define PUBLISH_INTERVAL  (5U) /* seconds */
#define TOPIC_NAME        "device/state"
#define STATE_MESSAGE     "work"

/* EMCUTE client stack size */
#define EMCUTE_PRIO       (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[EMCUTE_STACK_SIZE];
static msg_t queue[8];

static emcute_topic_t topic;

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_run(BROKER_PORT, stack, EMCUTE_STACK_SIZE, EMCUTE_PRIO);
    return NULL; /* shouldn't be reached */
}

int main(void) {
    /* Start EMCUTE thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    /* Setup remote address */
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    if (inet_pton(AF_INET, BROKER_IP, &gw.addr) != 1) {
        printf("Error: unable to parse broker address\n");
        return 1;
    }

    /* Connect to gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway at %s\n", BROKER_IP);
        return 1;
    }
    printf("Connected to %s\n", BROKER_IP);

    /* Setup topic */
    topic.name = TOPIC_NAME;
    emcute_reg(&topic);

    while (1) {
        /* Publish state message */
        if (emcute_pub(&topic, STATE_MESSAGE, strlen(STATE_MESSAGE), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        else {
            printf("Published: %s\n", STATE_MESSAGE);
        }

        /* Sleep for defined interval */
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}