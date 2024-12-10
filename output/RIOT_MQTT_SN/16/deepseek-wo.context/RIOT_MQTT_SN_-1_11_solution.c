
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
static emcute_sub_t sub;
static emcute_topic_t topic;

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    printf("Received message on topic '%s' [%i]: %.*s\n",
           topic->name, (int)topic->id, (int)len, (char *)data);
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    /* Initialize the EMCUTE thread */
    emcute_run(0, "RIOT-MQTT");

    /* Connect to the MQTT-SN broker */
    int res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error on connection: %d\n", res);
        return 1;
    }
    printf("Connected to MQTT-SN broker\n");

    /* Register the topic */
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error on registering topic: %d\n", res);
        return 1;
    }
    printf("Registered topic: %s with ID: %u\n", topic.name, topic.id);

    /* Subscribe to the topic */
    sub.topic = topic;
    sub.cb = _on_pub;
    res = emcute_sub(&sub, EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error on subscribing to topic: %d\n", res);
        return 1;
    }
    printf("Subscribed to topic: %s\n", topic.name);

    /* Publish state every 5 seconds */
    char msg[] = "work";
    while (1) {
        res = emcute_pub(&topic, msg, sizeof(msg) - 1, EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error on publishing message: %d\n", res);
        } else {
            printf("Published message: %s\n", msg);
        }
        xtimer_usleep(MSG_INTERVAL);
    }

    return 0;
}
