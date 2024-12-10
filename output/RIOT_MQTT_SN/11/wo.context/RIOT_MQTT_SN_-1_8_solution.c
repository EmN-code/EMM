#include <xtimer.h>
#include <string.h>
#include <net/mqttsn.h>
#include <msg.h>

#define EMCUTE_ID           "ESP32"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT         (1883)
#define EMCUTE_INTERFACE    ("eth0")
#define EMCUTE_BROKER_IP    "47.102.103.1"

#define STATE_MESSAGE       "work"
#define STATE_INTERVAL      (5U) // in seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t t_state = {
    .name = "state/topic",
    .id = 0,
};

static emcute_sub_t subscriptions[1];

static kernel_pid_t emcute_thread_pid;

int main(void) {
    // Initialize message queue
    msg_init_queue(queue, ARRAY_SIZE(queue));

    // Initialize and configure emcute
    emcute_thread_pid = thread_create(stack, sizeof(stack), EMCUTE_PRIO,
                                      0, emcute_thread, EMCUTE_INTERFACE, "emcute");

    // Setup broker address
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, EMCUTE_BROKER_IP, &gw.addr.ipv4);

    // Connect to broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway\n");
        return 1;
    }

    // Register topic
    if (emcute_reg(&t_state) != EMCUTE_OK) {
        printf("Error: unable to obtain topic ID\n");
        return 1;
    }

    // Set up periodic message publication
    while (1) {
        printf("Publishing message: %s\n", STATE_MESSAGE);
        if (emcute_pub(&t_state, STATE_MESSAGE, strlen(STATE_MESSAGE), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish\n");
        }
        xtimer_sleep(STATE_INTERVAL);
    }
    return 0;
}