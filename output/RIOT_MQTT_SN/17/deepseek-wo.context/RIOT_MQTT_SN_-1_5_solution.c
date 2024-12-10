
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/inet_pton.h>
#include <emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define MSG_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char msg_buf[64];
static sock_udp_ep_t gw = { .port = SERVER_PORT };
static emcute_topic_t topic;

static void _publish_state(void)
{
    static int state = 0;
    const char *states[] = { "work", "not work" };

    sprintf(msg_buf, "%s", states[state]);
    state = !state; // Toggle state

    if (emcute_pub(&topic, msg_buf, strlen(msg_buf), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        printf("Published: %s\n", msg_buf);
    }
}

int main(void)
{
    // Initialize the network stack
    if (netdev_default == NULL) {
        puts("Error: no default network device found");
        return 1;
    }

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid server IP address");
        return 1;
    }

    // Initialize emcute
    emcute_run(SERVER_PORT, CLIENT_ID);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        _publish_state();
        xtimer_usleep(MSG_INTERVAL);
    }

    return 0;
}
