
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char state_msg[32];
static emcute_topic_t topic;

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    (void)data;
    (void)len;
}

static int _connect_to_broker(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: unable to parse broker IP address");
        return -1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return -1;
    }

    puts("Connected to MQTT-SN broker");
    return 0;
}

static int _register_topic(void)
{
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return -1;
    }

    puts("Topic registered");
    return 0;
}

static void _publish_state(void)
{
    static int state = 0;
    snprintf(state_msg, sizeof(state_msg), "State: %s", state ? "work" : "not work");
    state = !state;

    if (emcute_pub(&topic, state_msg, strlen(state_msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
        return;
    }

    puts("Message published");
}

int main(void)
{
    puts("Starting MQTT-SN client");

    if (_connect_to_broker() != 0) {
        return -1;
    }

    if (_register_topic() != 0) {
        return -1;
    }

    while (1) {
        _publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}