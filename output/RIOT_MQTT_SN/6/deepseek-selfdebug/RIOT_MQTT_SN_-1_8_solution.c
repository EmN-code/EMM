
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_LARGE)
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMQX_SERVER_PORT };
static emcute_topic_t topic;

static void publish_state(void)
{
    static const char *states[] = { "work", "not work" };
    static int state_index = 0;

    const char *state = states[state_index];
    state_index = (state_index + 1) % 2;

    printf("Publishing state: %s\n", state);

    int res = emcute_pub(&topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing state: %d\n", res);
    }
}

static void *mqtt_thread(void *arg)
{
    (void)arg;

    // Initialize the topic
    topic.name = TOPIC_NAME;
    topic.id = 0;

    // Connect to the MQTT-SN gateway
    int res = inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);
    if (res != 1) {
        printf("Error converting IP address\n");
        return NULL;
    }

    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN gateway: %d\n", res);
        return NULL;
    }

    // Register the topic
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        emcute_discon();
        return NULL;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    // Start the MQTT-SN thread
    thread_create(stack, sizeof(stack), MQTT_THREAD_PRIORITY,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt");

    return 0;
}
