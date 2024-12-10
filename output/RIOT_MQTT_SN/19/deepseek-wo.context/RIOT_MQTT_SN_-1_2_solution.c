
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work or not work"
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static sock_udp_ep_t gw = { .port = EMCUTE_PORT, .family = AF_INET };
static emcute_topic_t topic;

static void *emcute_thread(void *arg)
{
    (void)arg;

    /* set the remote endpoint to the MQTT-SN broker */
    gw.addr.ipv4[0] = 47;
    gw.addr.ipv4[1] = 102;
    gw.addr.ipv4[2] = 103;
    gw.addr.ipv4[3] = 1;

    /* connect to the MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    /* register the topic */
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    /* publish state every 5 seconds */
    while (1) {
        if (emcute_pub(&topic, STATE_MESSAGE, strlen(STATE_MESSAGE), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            puts("State published");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    /* start the emcute thread */
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    return 0;
}
