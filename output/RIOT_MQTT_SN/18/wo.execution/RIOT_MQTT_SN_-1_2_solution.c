#include "thread.h"
#include "event/thread.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"

#define MSG_TYPE_MQTT_PUBLISH (0x7010)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_q[8];
static kernel_pid_t emcute_thread_pid;

static emcute_topic_t topic;

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_q, 8);

    /* configure the topic we want to publish to */
    topic.name = "state";
    topic.id = 0;

    while (1) {
        /* sleep 5 seconds */
        xtimer_sleep(5); // Using xtimer to handle sleep timing

        /* Prepare the message to publish */
        const char *state = "work";

        /* publish the state */
        emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
    }
    return NULL;
}

int main(void)
{
    /* declare and configure sock_udp_ep_t-type endpoint structure */
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = 1883
    };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    /* start emcute thread */
    emcute_thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                      0, emcute_thread, NULL, "emcute");

    /* connect to MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to broker\n");
        return 1;
    }

    puts("Success: connected to broker\n");

    /* Let the main thread wait for messages */
    while (1) {
        msg_t m;
        msg_receive(&m);
    }
    return 0;
}