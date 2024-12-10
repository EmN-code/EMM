
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char *msgs[] = { "work", "not work" };
static uint8_t msg_idx = 0;

static void publish_state(void)
{
    emcute_topic_t topic;
    char payload[128];

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return;
    }

    // Prepare the payload
    snprintf(payload, sizeof(payload), "%s", msgs[msg_idx]);
    msg_idx = (msg_idx + 1) % ARRAY_SIZE(msgs);

    // Publish the message
    if (emcute_pub(&topic, payload, strlen(payload), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        printf("Published: %s\n", payload);
    }
}

int main(void)
{
    sock_udp_ep_t gw = { .port = SERVER_PORT };
    emcute_sub_t sub;
    xtimer_t timer;

    // Initialize the network stack
    if (net_init() < 0) {
        puts("Error: unable to initialize network stack");
        return 1;
    }

    // Convert the server IP address
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid server IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Set up the timer for periodic publishing
    timer.target = timer.long_target = 0;
    xtimer_set(&timer, MSG_INTERVAL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);

        if (msg.type == MSG_TYPE_TIMER_EXPIRED) {
            publish_state();
            xtimer_set(&timer, MSG_INTERVAL);
        }
    }

    return 0;
}
