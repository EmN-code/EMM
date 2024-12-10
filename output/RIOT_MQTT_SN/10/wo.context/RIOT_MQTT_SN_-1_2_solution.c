#include "net/emcute.h"
#include "net/af.h"
#include "msg.h"
#include "timex.h"
#include "ztimer.h"
#include "net/sock/udp.h"
#include "kernel.h"

#define EMCUTE_PORT     (1883U)
#define EMCUTE_ID       "esp32_client"
#define STATE_TOPIC     "state"
#define STATE_INTERVAL  (5U) // 5 seconds
#define SERVER_ADDR     "47.102.103.1"

static emcute_topic_t state_topic;
static sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };

static int publish_state(void) {
    int res = emcute_pub(&state_topic, "work", strlen("work"), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("error: unable to publish data (%d)\n", res);
        return res;
    }

    printf("Published state to topic '%s'\n", state_topic.name);
    return EMCUTE_OK;
}

int main(void) {
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    gw.port = EMCUTE_PORT;
    inet_pton(AF_INET6, SERVER_ADDR, &gw.addr.ipv6);

    // Connect to broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to the gateway\n");
        return 1;
    }
    puts("Connected to MQTT-SN gateway\n");

    // Setup topic
    state_topic.name = STATE_TOPIC;
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        puts("error: unable to obtain topic\n");
        return 1;
    }

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_SEC, STATE_INTERVAL);
    }

    // Disconnect
    if (emcute_discon() != EMCUTE_OK) {
        puts("error: unable to disconnect from the gateway\n");
    }

    return 0;
}