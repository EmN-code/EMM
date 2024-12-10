#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"
#include "ztimer.h"

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "device/state"
#define STATE_MSG "work or not work"
#define SLEEP_INTERVAL 5

static emcute_topic_t topic;

static int connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    gw.addr.ipv4[0] = 47;
    gw.addr.ipv4[1] = 102;
    gw.addr.ipv4[2] = 103;
    gw.addr.ipv4[3] = 1;

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to gateway [%d]\n", res);
        return res;
    }

    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic [%d]\n", res);
        return res;
    }
    return EMCUTE_OK;
}

int main(void) {
    int res = connect_to_broker();
    if (res != EMCUTE_OK) {
        return res;
    }

    while (1) {
        const char *data = STATE_MSG;
        size_t len = strlen(data);
        res = emcute_pub(&topic, data, len, 0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish data [%d]\n", res);
            // Decide whether to retry or exit
        }
        ztimer_sleep(ZTIMER_MSEC, SLEEP_INTERVAL * 1000);
    }

    return 0;
}