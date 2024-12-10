#include <thread.h>
#include <net/emcute.h>
#include <event/thread.h>
#include <netinet/in.h>
#include <net/gcoap.h>

#define EMCUTE_ID           "my_id"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_STACK_SIZE   (THREAD_STACKSIZE_DEFAULT)
#define BROKER_IP           "47.102.103.1"
#define BROKER_PORT         1883
#define PUBLISH_INTERVAL    (5U)  // publish every 5 seconds

static char stack[EMCUTE_STACK_SIZE];
static msg_t queue[8];

static void *emcute_thread(void *arg) {
    /* setup the UDP endpoint */
    sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };
    inet_pton(AF_INET6, BROKER_IP, &gw.addr.ipv6);

    /* initialize the emcute network stack */
    emcute_run(THREAD_CREATE_STACKTEST, stack, sizeof(stack), EMCUTE_PRIO, "emcute", (void*) EMCUTE_ID);

    /* try to connect to the broker defined in the `gw` endpoint */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Could not connect to gateway at [%s]:%d\n", BROKER_IP, BROKER_PORT);
        return NULL;
    }

    /* start publishing data to the topic */
    emcute_topic_t t;
    t.name = "device/state";

    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("Could not register topic\n");
        return NULL;
    }

    while (1) {
        const char *state = "work or not work";

        if (emcute_pub(&t, state, strlen(state), 0) != EMCUTE_OK) {
            printf("error: unable to publish to topic %s\n", t.name);
        }
        else {
            printf("Published %s to topic %s\n", state, t.name);
        }

        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    /* setup the message queue */
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    return 0;
}