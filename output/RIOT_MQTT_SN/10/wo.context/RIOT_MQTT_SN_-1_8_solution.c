#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/emcute.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static emcute_topic_t topic;
static char topic_name[] = "state";
static sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };

static int _publish(char *message) {
    return emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
}

static void *_publisher(void *arg) {
    (void)arg;
    char *state_msg[2] = {"work", "not work" };
    int state = 0;

    while (1) {
        _publish(state_msg[state]);
        state = (state + 1) % 2;
        xtimer_sleep(PUBLISH_INTERVAL);
    }
    return NULL;
}

static int _connect(void) {
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: Unable to connect to [%s]:%u\n", BROKER_IP, BROKER_PORT);
        return 1;
    }

    topic.name = topic_name;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: Unable to register topic \"%s\"\n", topic_name);
        return 1;
    }

    return 0;
}

int main(void) {
    if (_connect() != 0) {
        return 1;
    }

    _publisher(NULL);

    return 0;
}