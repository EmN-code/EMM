
#include "net/emcute.h"
#include "xtimer.h"
#include "stdbool.h"
#include "net/ipv6/addr.h"
#include "stdlib.h"
#include "thread.h"
#include "msg.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work or not work"
#define STATE_MESSAGE_LEN (sizeof(STATE_MESSAGE) - 1)
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static emcute_topic_t topic;
static sock_udp_ep_t remote;

static void *emcute_thread(void *arg)
{
    (void)arg;

    /* Initialize the remote endpoint */
    remote.family = AF_INET;
    remote.port = EMQX_SERVER_PORT;
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    /* Connect to the MQTT-SN gateway */
    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN gateway");
        return NULL;
    }

    /* Register the topic */
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: could not register topic");
        return NULL;
    }

    /* Publish state every 5 seconds */
    while (1) {
        if (emcute_pub(&topic, STATE_MESSAGE, STATE_MESSAGE_LEN, 0) != EMCUTE_OK) {
            puts("Error: could not publish message");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    /* Start the EMCUTE thread */
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    return 0;
}
