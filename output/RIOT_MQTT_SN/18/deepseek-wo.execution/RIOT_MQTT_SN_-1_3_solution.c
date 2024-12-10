
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
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define MSG_LEN 5
#define DELAY (5 * 1000000U)  // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };

static void _publish_state(void)
{
    static const char *msg = MSG_WORK;
    static bool work = true;

    if (emcute_pub(&topic, msg, MSG_LEN, 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    }

    // Toggle state
    work = !work;
    msg = work ? MSG_WORK : MSG_NOT_WORK;
}

int main(void)
{
    // Initialize the topic structure
    topic.name = TOPIC_NAME;
    topic.id = 0;

    // Set the gateway address
    inet_aton("47.102.103.1", &gw.addr.ipv4);

    // Connect to the MQTT-SN gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN gateway");
        return 1;
    }

    // Register the topic
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        _publish_state();
        ztimer_sleep(ZTIMER_USEC, DELAY);
    }

    return 0;
}
