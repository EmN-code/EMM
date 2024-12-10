
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t topic;

static void *publish_state(void *arg)
{
    const char *states[] = { "work", "not work" };
    unsigned int state_index = 0;

    while (1) {
        /* Prepare the message */
        const char *state = states[state_index];
        printf("Publishing state: %s\n", state);

        /* Publish the state */
        emcute_pub(&topic, state, strlen(state), 0);

        /* Toggle state */
        state_index = (state_index + 1) % (sizeof(states) / sizeof(states[0]));

        /* Wait for the next publish interval */
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    /* Initialize the network interface */
    if (netif_set_default(netif_iter(NULL)) == NULL) {
        puts("No default network interface found");
        return 1;
    }

    /* Set the gateway endpoint */
    if (ipv4_addr_from_str("47.102.103.1", (ipv4_addr_t *)&gw.addr.ipv4) == NULL) {
        puts("Error parsing gateway IP address");
        return 1;
    }

    /* Connect to the MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    /* Register the topic */
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }
    printf("Registered topic: %s\n", topic.name);

    /* Start the publish thread */
    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, publish_state, NULL, "publish_state");

    /* Run the EMCUTE event loop */
    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    return 0;
}
