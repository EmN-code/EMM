
#include <net/emcute.h>
#include <net/sock/util.h>
#include <net/ipv6/addr.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <sys/socket.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_CONTENT "work or not work"
#define MSG_LEN sizeof(MSG_CONTENT) - 1

static emcute_topic_t topic;

static void _register_topic(void) {
    topic.name = TOPIC_NAME;
    emcute_reg(&topic);
}

static void _publish_state(void) {
    emcute_pub(&topic, MSG_CONTENT, MSG_LEN, 0);
}

int main(void) {
    sock_udp_ep_t gw = SOCK_IPV6_EP_ANY;
    gw.port = BROKER_PORT;
    gw.family = AF_INET;

    if (sock_udp_str2ep(&gw, BROKER_ADDR) < 0) {
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        return 1;
    }

    _register_topic();

    while (1) {
        _publish_state();
        sleep(5);
    }

    return 0;
}
