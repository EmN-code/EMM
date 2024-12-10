
#include <stdlib.h>
#include <mutex.h>
#include <net/emcute.h>
#include <stdio.h>
#include <sys/socket.h>
#include <event/thread.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <thread.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .port = BROKER_PORT, .family = AF_INET };

static void *publish_thread(void *arg)
{
    (void)arg;
    const char *states[] = { "work", "not work" };
    int state_index = 0;

    while (1) {
        // Prepare the message
        const char *state = states[state_index];
        state_index = (state_index + 1) % 2;

        // Publish the state
        if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    // Initialize the topic
    topic.name = TOPIC_NAME;
    topic.id = 0;

    // Set up the gateway endpoint
    if (sock_udp_str2ep(&gw, BROKER_ADDR) < 0) {
        puts("Error: unable to parse broker address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Start the publish thread
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publish_thread, NULL, "publisher");

    // Run the emCute event loop
    emcute_run(BROKER_PORT, CLIENT_ID);

    return 0;
}
