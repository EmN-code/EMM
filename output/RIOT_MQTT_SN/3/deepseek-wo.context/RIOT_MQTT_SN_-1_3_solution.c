
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define MSG_SIZE 128
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char msg_buf[MSG_SIZE];
static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };

static void publish_state(void)
{
    static int state = 0;
    const char *states[] = { "work", "not work" };

    snprintf(msg_buf, sizeof(msg_buf), "%s", states[state]);
    state = !state;

    emcute_topic_t t;
    t.name = TOPIC;
    t.id = 0;  // Topic ID will be assigned by the broker

    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return;
    }

    if (emcute_pub(&t, msg_buf, strlen(msg_buf), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        printf("Published: %s\n", msg_buf);
    }
}

int main(void)
{
    // Initialize the network stack
    if (net_init() < 0) {
        puts("Error: cannot initialize network stack");
        return 1;
    }

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid server IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    puts("Connected to MQTT-SN broker");

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    // Disconnect from the MQTT-SN broker
    emcute_discon();

    return 0;
}
