
#include "net/emcute.h"
#include "netinet/in.h"
#include "net/ipv6/addr.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work or not work"
#define TIME_INTERVAL (5 * 1000000UL) // 5 seconds in microseconds

static emcute_topic_t topic;

static void _publish_state(void)
{
    const char *message = STATE_MESSAGE;
    size_t len = strlen(message);
    int res = emcute_pub(&topic, message, len, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish message: %d\n", res);
    }
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN broker: %d\n", res);
        return 1;
    }

    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    topic.name[sizeof(topic.name) - 1] = '\0';

    while (1) {
        _publish_state();
        xtimer_usleep(TIME_INTERVAL);
    }

    return 0;
}
