#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/mqttsn.h>
#include <msg.h>
#include <net/sock/udp.h>

#define EMCUTE_PORT     (1883U)
#define EMCUTE_ID       ("my_mqtt_client")
#define STATE_TOPIC     ("state")
#define WORK_STATE      ("work")
#define NOT_WORK_STATE  ("not work")
#define PUB_INTERVAL    (5U) // publish every 5 seconds

static msg_t msg_q[8];  // Declare the message queue
static char stack[THREAD_STACKSIZE_DEFAULT];  // Declare the stack

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

int main(void) {
    /* initialize the message queue */
    msg_init_queue(msg_q, sizeof(msg_q) / sizeof(msg_q[0]));

    /* start emcute in a separate thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    /* set remote address */
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    /* endless publishing loop */
    while (1) {
        char *state = WORK_STATE;  // Change to NOT_WORK_STATE when appropriate

        /* prepare topic struct */
        emcute_topic_t t;
        memset(&t, 0, sizeof(t));
        t.name = STATE_TOPIC;

        /* publish the state */
        if (emcute_pub(&t, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish");
        }

        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}