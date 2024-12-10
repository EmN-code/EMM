#include <xtimer.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/emcute.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID   "esp32_client"
#define TOPIC_NAME  "device/state"
#define PUBLISH_INTERVAL 5

static emcute_topic_t topic;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;
    printf("Received publication: Topic: %s, Data: %s\n", topic->name, in);
}

int main(void)
{
    msg_t queue[8];
    msg_init_queue(queue, ARRAY_SIZE(queue));

    printf("Connecting to the gateway...\n");

    /* Connect to the MQTT-SN gateway */
    sock_udp_ep_t gw = { .family = AF_INET,
                         .port = SERVER_PORT };
    ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, SERVER_ADDR);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway %s\n", SERVER_ADDR);
        return 1;
    }

    printf("Connected!");

    /* Subscribe to a topic */
    memset(&topic, 0, sizeof(emcute_topic_t));
    topic.name = TOPIC_NAME;
    if (emcute_sub(&topic, EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("Unable to subscribe to %s\n", TOPIC_NAME);
        return 1;
    }
    printf("Subscribed to %s\n", TOPIC_NAME);

    /* Publish loop */
    while (1) {
        const char *msg = "work";   // or "not work" based on the state
        if (emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Unable to publish data\n");
            return 1;
        }
        printf("Published: %s\n", msg);
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    /* Disconnect from the gateway */
    emcute_discon();
    printf("Disconnected!\n");

    return 0;
}