
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "thread.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ID         "riot_esp32"
#define TOPIC_NAME        "state"
#define STATE_MESSAGE     "work or not work"
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t topic;

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMCUTE_PORT };

    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    if (sock_udp_create(&local, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return NULL;
    }

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return NULL;
    }

    while (1) {
        if (emcute_pub(&topic, STATE_MESSAGE, strlen(STATE_MESSAGE), 0) != EMCUTE_OK) {
            puts("Error publishing message");
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
