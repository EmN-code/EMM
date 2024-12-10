
#include <stdlib.h>
#include <stdbool.h>
#include <time_units.h>
#include <ztimer.h>
#include <net/emcute.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work"
#define MESSAGE_LEN 4
#define SLEEP_INTERVAL (5 * US_PER_SEC)

static emcute_topic_t topic;

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN gateway");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    while (1) {
        if (emcute_pub(&topic, MESSAGE, MESSAGE_LEN, 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }
        ztimer_sleep(ZTIMER_USEC, SLEEP_INTERVAL);
    }

    return 0;
}
