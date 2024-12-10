
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
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char state_msg[] = "work";
static emcute_topic_t topic;

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    (void)data;
    (void)len;
    printf("Received message on topic %s\n", topic->name);
}

int main(void)
{
    sock_udp_ep_t gw = { .port = SERVER_PORT };
    emcute_sub_t sub;
    msg_t msg, reply;

    // Initialize the RIOT network stack
    if (netdev_default == NULL) {
        puts("No default network device found");
        return 1;
    }

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    // Initialize the EMCUTE stack
    emcute_run(SERVER_PORT, CLIENT_ID);

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
    printf("Registered topic: %s\n", topic.name);

    // Subscribe to the topic
    sub.topic = &topic;
    sub.cb = _on_pub;
    if (emcute_sub(&sub, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error subscribing to topic");
        return 1;
    }
    printf("Subscribed to topic: %s\n", topic.name);

    // Publish state every 5 seconds
    while (1) {
        if (emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            printf("Published: %s\n", state_msg);
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
