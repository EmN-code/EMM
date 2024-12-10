#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL (5U * 1000U * 1000U)  // 5 seconds in microseconds
#define PUBLISH_TOPIC "device/state"

static emcute_topic_t state_topic;
static char *state_message = "work";

static int connect_to_broker(sock_udp_ep_t *gw)
{
    gw->family = AF_INET;
    gw->port = SERVER_PORT;
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw->addr.ipv4, SERVER_ADDR) == NULL) {
        puts("Unable to parse IPv4 address");
        return EMCUTE_NOGW;
    }
    return emcute_con(gw, true, NULL, NULL, 0, 0);
}

static void publish_state(void)
{
    printf("Publishing state: %s\n", state_message);
    emcute_pub(&state_topic, state_message, strlen(state_message), EMCUTE_QOS_0);
}

int main(void)
{
    sock_udp_ep_t gw;
    state_topic.name = PUBLISH_TOPIC;
    if (connect_to_broker(&gw) != EMCUTE_OK) {
        puts("Connection to MQTT-SN broker failed!");
        return 1;
    }
    emcute_reg(&state_topic);

    while (1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}