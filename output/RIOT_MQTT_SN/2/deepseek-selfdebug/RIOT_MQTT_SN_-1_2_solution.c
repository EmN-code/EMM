
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define MSG_SIZE 128
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char msg_buf[MSG_SIZE];
static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
static emcute_topic_t topic;

static void publish_state(void)
{
    static int state = 0;
    const char *states[] = { "work", "not work" };

    snprintf(msg_buf, sizeof(msg_buf), "%s", states[state]);
    state = !state;  // Toggle state

    if (emcute_pub(&topic, msg_buf, strlen(msg_buf), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        printf("Published: %s\n", msg_buf);
    }
}

int main(void)
{
    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_t sock;

    // Initialize UDP socket
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Set gateway address
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error parsing server IP");
        return 1;
    }

    // Connect to MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    // Register topic
    topic.name = TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    // Disconnect from MQTT-SN broker
    emcute_discon();
    puts("Disconnected from MQTT-SN broker");

    return 0;
}
