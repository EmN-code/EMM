#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mma8x5x_regs.h>
#include <event/thread.h>
#include <thread.h>
#include <net/gcoap.h>

#define EMCUTE_ID     "mqtt-client"
#define EMCUTE_PORT   (1884U)
#define EMCUTE_PRIO   (THREAD_PRIORITY_MAIN - 1)
#define EMQX_SERVER_PORT 1883
#define EMQX_SERVER_IP "47.102.103.1"

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

/* The MQTT-SN topic we will subscribe and publish to */
static emcute_topic_t topic;

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT);
    return NULL;
}

int main(void) {
    /* some buffers for our connections and message queue */
    memset(queue, 0, sizeof(queue));

    /* initialize the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* convert IP address */
    sock_udp_ep_t remote;
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = EMQX_SERVER_PORT;

    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse destination address");
        return 1;
    }

    /* connect to MQTT-SN broker */
    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }

    /* set MQTT-SN topic */
    topic.name = "device_status";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    /* Publish message */
    while (1) {
        char *state = "work"; // or "not work" based on your condition
        if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }
        // Here we need to sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}