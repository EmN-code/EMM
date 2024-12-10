
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;

void publish_state(void) {
    static bool state = false;
    const char *msg = state ? MSG_WORK : MSG_NOT_WORK;
    state = !state;

    int res = emcute_pub(&topic, msg, strlen(msg), 0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish message: %d\n", res);
    } else {
        printf("Published: %s\n", msg);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, BROKER_ADDR) == NULL) {
        printf("Error parsing broker address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker\n");
        return 1;
    }

    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
