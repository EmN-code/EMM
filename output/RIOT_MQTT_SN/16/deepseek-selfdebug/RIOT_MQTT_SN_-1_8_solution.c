
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char state_msg[] = "work or not work";
static emcute_topic_t topic;

static void _register_topic(void)
{
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return;
    }
    printf("Registered topic: %s with ID: %u\n", topic.name, (unsigned)topic.id);
}

static void _publish_state(void)
{
    if (emcute_pub(&topic, state_msg, strlen(state_msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        puts("State published");
    }
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_BROKER_PORT };
    inet_pton(AF_INET, MQTT_SN_BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    _register_topic();

    while (1) {
        _publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
