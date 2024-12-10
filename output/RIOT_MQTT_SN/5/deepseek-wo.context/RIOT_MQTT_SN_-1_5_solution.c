
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "shell.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ID         "riot_esp32"
#define TOPIC_NAME        "state"
#define PUBLISH_INTERVAL  (5 * 1000000U)  /* 5 seconds in microseconds */

static emcute_topic_t topic;
static sock_udp_ep_t gw;

static void publish_state(void)
{
    static const char *states[] = {"work", "not work"};
    static unsigned state_index = 0;

    const char *state = states[state_index];
    state_index = (state_index + 1) % ARRAY_SIZE(states);

    printf("Publishing state: %s\n", state);

    if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
    }
}

static void *emcute_thread(void *arg)
{
    (void)arg;

    /* Initialize the topic */
    topic.name = TOPIC_NAME;
    topic.id = 0;

    /* Initialize the gateway endpoint */
    if (sock_udp_ep_from_str(&gw, "47.102.103.1", EMCUTE_PORT) < 0) {
        puts("Error: unable to parse gateway address");
        return NULL;
    }

    /* Connect to the MQTT-SN gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN gateway");
        return NULL;
    }

    /* Register the topic */
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        emcute_discon();
        return NULL;
    }

    /* Publish state every 5 seconds */
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    /* Start the emcute thread */
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    /* Run the shell to allow manual interaction */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
