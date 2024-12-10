
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static emcute_sub_t sub;
static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

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
    if (netdev_default == NULL) {
        puts("No default network device found");
        return 1;
    }

    // Initialize the EMCUTE stack
    emcute_init();

    // Set up the gateway endpoint
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, BROKER_ADDR) == NULL) {
        puts("Error parsing broker address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    // Register the topic
    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    // Subscribe to the topic
    sub.topic = topic;
    sub.cb = on_pub;
    if (emcute_sub(&sub, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error subscribing to topic");
        return 1;
    }

    // Publish state every 5 seconds
    char msg[10];
    while (1) {
        // Publish "work"
        strcpy(msg, MSG_WORK);
        if (emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            printf("Published: %s\n", msg);
        }

        xtimer_usleep(INTERVAL);

        // Publish "not work"
        strcpy(msg, MSG_NOT_WORK);
        if (emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            printf("Published: %s\n", msg);
        }

        xtimer_usleep(INTERVAL);
    }

    return 0;
}
