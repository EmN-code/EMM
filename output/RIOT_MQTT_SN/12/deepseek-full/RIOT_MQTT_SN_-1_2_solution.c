
#include "net/emcute.h"
#include "net/sock/util.h"
#include "net/ipv6/addr.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "sys/socket.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_CONTENT "work or not work"
#define MSG_LEN sizeof(MSG_CONTENT)

static emcute_topic_t topic;
static sock_udp_ep_t gw;

static void _publish_state(void)
{
    int res;
    res = emcute_pub(&topic, MSG_CONTENT, MSG_LEN, 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing message: %d\n", res);
    }
}

int main(void)
{
    int res;

    /* Initialize the UDP endpoint for the gateway */
    gw.family = AF_INET;
    gw.port = SERVER_PORT;
    res = inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);
    if (res != 1) {
        printf("Error converting IP address\n");
        return 1;
    }

    /* Connect to the MQTT-SN gateway */
    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to gateway: %d\n", res);
        return 1;
    }

    /* Register the topic */
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        emcute_discon();
        return 1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        _publish_state();
        xtimer_sleep(5);
    }

    /* Disconnect from the gateway */
    emcute_discon();

    return 0;
}
