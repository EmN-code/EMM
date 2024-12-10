
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define EMCUTE_PORT 1883
#define EMCUTE_ADDR "47.102.103.1"
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char state = 0; // 0 for "not work", 1 for "work"

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    (void)data;
    (void)len;
    printf("Received message on topic %s\n", topic->name);
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    emcute_topic_t topic;
    emcute_sub_t sub;
    char buf[128];

    // Initialize the network stack
    if (net_init() < 0) {
        puts("Error initializing network stack");
        return 1;
    }

    // Convert IP address string to network byte order
    if (inet_pton(AF_INET, EMCUTE_ADDR, &gw.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }
    puts("Topic registered");

    // Subscribe to the topic
    sub.topic = topic;
    sub.cb = on_pub;
    if (emcute_sub(&sub, 0) != EMCUTE_OK) {
        puts("Error subscribing to topic");
        return 1;
    }
    puts("Subscribed to topic");

    // Publish state every 5 seconds
    while (1) {
        if (state) {
            strncpy(buf, MSG_WORK, sizeof(buf));
        } else {
            strncpy(buf, MSG_NOT_WORK, sizeof(buf));
        }
        state = !state; // Toggle state

        if (emcute_pub(&topic, buf, strlen(buf), 0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            printf("Published: %s\n", buf);
        }

        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Disconnect from the MQTT-SN broker
    if (emcute_discon() != EMCUTE_OK) {
        puts("Error disconnecting from MQTT-SN broker");
        return 1;
    }
    puts("Disconnected from MQTT-SN broker");

    return 0;
}
