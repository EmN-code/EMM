
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

#define EMCUTE_PORT       1883
#define EMCUTE_ID         "riot_esp32"
#define TOPIC_NAME        "state"
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static char state_msg[] = "work";

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t gw = { .port = EMCUTE_PORT, .family = AF_INET };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    emcute_topic_t topic;
    if (emcute_reg(&topic, TOPIC_NAME) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    while (1) {
        if (emcute_pub(&topic, state_msg, strlen(state_msg), 0) != EMCUTE_OK) {
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
    puts("RIOT MQTT-SN example");

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute_thread");

    return 0;
}
