
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "xtimer.h"
#include "thread.h"

#define EMCUTE_PORT       1883
#define EMCUTE_BUFSIZE    128
#define EMCUTE_TOPIC      "state"
#define EMCUTE_MSG        "work or not work"
#define EMCUTE_MSG_LEN    (sizeof(EMCUTE_MSG) - 1)
#define EMCUTE_QOS        0
#define EMCUTE_RETAIN     0
#define EMCUTE_INTERVAL   (5 * 1000000U)  /* 5 seconds in microseconds */

static emcute_topic_t topic;

static void _register_topic(void) {
    topic.name = EMCUTE_TOPIC;
    topic.id = 0;  /* Topic ID will be assigned by the broker */
}

static void _publish_state(void) {
    int res = emcute_pub(&topic, EMCUTE_MSG, EMCUTE_MSG_LEN, EMCUTE_QOS | EMCUTE_RETAIN);
    if (res != EMCUTE_OK) {
        printf("Failed to publish message: %d\n", res);
    } else {
        printf("Published: %s\n", EMCUTE_MSG);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .port = EMCUTE_PORT, .family = AF_INET };
    inet_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, "47.102.103.1");

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker: %d\n", res);
        return 1;
    }

    _register_topic();

    while (1) {
        _publish_state();
        xtimer_usleep(EMCUTE_INTERVAL);
    }

    return 0;
}
