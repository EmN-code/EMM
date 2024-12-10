
#include <net/emcute.h>
#include <xtimer.h>
#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .port = EMQX_SERVER_PORT };

static void publish_state(void)
{
    static bool state = true;
    const char *msg = state ? MSG_WORK : MSG_NOT_WORK;
    state = !state;

    int res = emcute_pub(&topic, msg, strlen(msg), 0);
    if (res != EMCUTE_OK) {
        puts("Error publishing message");
    } else {
        printf("Published: %s\n", msg);
    }
}

int main(void)
{
    /* Initialize the topic structure */
    topic.name = TOPIC_NAME;
    topic.id = 0;

    /* Convert IP address string to byte representation */
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4) != 1) {
        puts("Error converting IP address");
        return 1;
    }

    /* Connect to the MQTT-SN gateway */
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN gateway");
        return 1;
    }

    /* Register the topic */
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
