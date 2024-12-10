
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static emcute_sub_t sub;
static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };

static void on_pub(void *arg, const emcute_topic_t *topic, void *data, size_t len)
{
    (void)arg;
    (void)topic;
    (void)data;
    (void)len;
    printf("Received message on topic: %s\n", topic->name);
}

int main(void)
{
    // Initialize the network stack
    if (net_init() < 0) {
        puts("Error initializing network stack");
        return 1;
    }

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    // Initialize EMCUTE
    emcute_init();

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    // Subscribe to the topic
    sub.topic.name = TOPIC_NAME;
    sub.cb = on_pub;
    if (emcute_sub(&sub, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error subscribing to topic");
        return 1;
    }

    // Publish state every 5 seconds
    char *msg = MSG_WORK;
    while (1) {
        if (emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            printf("Published: %s\n", msg);
            msg = (strcmp(msg, MSG_WORK) == 0) ? MSG_NOT_WORK : MSG_WORK;
        }
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
