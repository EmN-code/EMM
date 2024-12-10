#include <ztimer.h>
#include <string.h>
#include <stdio.h>
#include <emcute.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/ipv4/addr.h>

#define EMCUTE_ID          "my_esp32_node"
#define EMCUTE_PORT        1883
#define EMCUTE_TOPIC_LEN   64

static emcute_topic_t my_topic;
static sock_udp_ep_t broker_endpoint;

void publish_state(void) {
    const char *data = "work";
    if (emcute_pub(&my_topic, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("Error: unable to publish data\n");
    }
}

int main(void) {
    /* set broker address and port */
    broker_endpoint.family = AF_INET;
    inet_pton(AF_INET, "47.102.103.1", &broker_endpoint.addr.ipv4);
    broker_endpoint.port = EMCUTE_PORT;

    /* connect to broker */
    if (emcute_con(&broker_endpoint, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to broker\n");
        return 1;
    }

    /* initialize topic */
    my_topic.name = "state";

    /* register topic */
    if (emcute_reg(&my_topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}