
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define MSG_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char state_msg[] = "work";
static emcute_topic_t topic;

static void publish_state(void)
{
    if (emcute_pub(&topic, state_msg, strlen(state_msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        puts("State published");
    }
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    emcute_sub_t sub;

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid server IP address");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN gateway");
        return 1;
    }
    puts("Connected to MQTT-SN gateway");

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }
    printf("Registered topic: %s with ID %u\n", topic.name, topic.id);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(MSG_INTERVAL);
    }

    // Disconnect from the MQTT-SN gateway
    if (emcute_discon() != EMCUTE_OK) {
        puts("Error: unable to disconnect from MQTT-SN gateway");
    }
    puts("Disconnected from MQTT-SN gateway");

    return 0;
}