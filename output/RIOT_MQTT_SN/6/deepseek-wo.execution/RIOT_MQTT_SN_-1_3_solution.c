
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <mma8x5x_regs.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>

#define TOPIC_NAME "state"
#define MSG_CONTENT "work"
#define MSG_LEN sizeof(MSG_CONTENT)
#define GATEWAY_IP "47.102.103.1"
#define GATEWAY_PORT 1883
#define SLEEP_TIME (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;

int main(void)
{
    // Initialize the EMCUTE library
    emcute_init();

    // Set up the remote endpoint for the MQTT-SN gateway
    sock_udp_ep_t gw = { .family = AF_INET, .port = GATEWAY_PORT };
    inet_pton(AF_INET, GATEWAY_IP, &gw.addr.ipv4);

    // Connect to the MQTT-SN gateway
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        return 1;
    }

    // Publish the state every 5 seconds
    while (1) {
        res = emcute_pub(&topic, MSG_CONTENT, MSG_LEN, 0);
        if (res != EMCUTE_OK) {
            return 1;
        }
        ztimer_sleep(ZTIMER_USEC, SLEEP_TIME);
    }

    return 0;
}
