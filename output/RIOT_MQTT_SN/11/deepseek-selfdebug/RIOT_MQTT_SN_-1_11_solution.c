
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define MQTT_SN_TOPIC "state"
#define MQTT_SN_CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char state_message[20];
static emcute_topic_t topic;

static void publish_state(void)
{
    static int state = 0;
    const char *states[] = {"work", "not work"};

    sprintf(state_message, "%s", states[state]);
    state = !state;

    if (emcute_pub(&topic, state_message, strlen(state_message), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        printf("Published: %s\n", state_message);
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

    topic.name = MQTT_SN_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }
    printf("Registered topic: %s with ID %u\n", topic.name, topic.id);

    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
