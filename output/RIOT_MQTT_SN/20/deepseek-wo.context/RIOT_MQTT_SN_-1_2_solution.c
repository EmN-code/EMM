
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <emcute.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"
#define MSG_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char state_msg[] = "work";
static emcute_topic_t topic;

static void publish_state(void)
{
    if (emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        puts("State published");
    }
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    topic.name = TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }
    puts("Topic registered");

    xtimer_t timer;
    timer.target = timer.long_target = xtimer_now();
    while (1) {
        publish_state();
        timer.target += MSG_INTERVAL;
        xtimer_set(&timer, MSG_INTERVAL);
        xtimer_usleep_until(&timer.target);
    }

    return 0;
}
