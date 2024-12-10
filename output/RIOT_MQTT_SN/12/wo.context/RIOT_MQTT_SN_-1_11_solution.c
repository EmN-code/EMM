#include <stdlib.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/mqttsn.h"
#include "emcute.h"
#include "xtimer.h"
#include "thread.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define PUB_TOPIC_NAME "state"
#define PUB_INTERVAL (5U * US_PER_SEC)

static emcute_topic_t topic;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{   //callback on receiving publishments
    (void)topic;
    (void)data;
    (void)len;
}

static int connect_to_broker(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .netif = SOCK_ADDR_ANY_NETIF }; // define remote endpoint
    gw.port = SERVER_PORT;
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, SERVER_ADDR) == NULL) {
       printf("Error parsing server address\n");
        return -1;
    }

    //connect to MQTT-SN broker
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to the broker.\n");
        return res;
    }

    //prepare the topic we want to publish to
    topic.name = PUB_TOPIC_NAME;
    if ((res = emcute_reg(&topic)) != EMCUTE_OK) {
        printf("Error: unable to obtain topic ID.\n");
        return res;
    }

    return EMCUTE_OK;
}

int main(void)
{
    //initialize the emcute stack
    if (connect_to_broker() != EMCUTE_OK) {
        printf("Failed to setup MQTT-SN connection\n");
        return 1;
    }

    char *data = "work";
    size_t len = strlen(data);

    while (1) {
        int res = emcute_pub(&topic, data, len, EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish data.\n");
        }
        xtimer_usleep(PUB_INTERVAL);
    }

    return 0;
}