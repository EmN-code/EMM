#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/ipv6/udp.h>
#include "msg.h"

#define EMCUTE_ID           "station"
#define EMCUTE_PORT         1883
#define EMCUTE_TOPIC        "state"
#define EMCUTE_MSG_WORK     "work"
#define EMCUTE_MSG_NO_WORK  "not work"
#define SLEEP_INTERVAL_SEC  5

static msg_t queue[8];

int main(void) {
    /* initialize our emmqtt sn client */
    emcute_run(CONFIG_GCOAP_PDU_BUF_SIZE, EMCUTE_ID);

    /* configure the server address and port */
    sock_udp_ep_t gw = {
        .family = AF_INET6,
        .port = EMCUTE_PORT
    };
    ipv6_addr_t addr;
    inet_pton(AF_INET6, "47.102.103.1", &addr);
    memcpy(&gw.addr, &addr, sizeof(addr));

    /* connect to MQTT broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to the broker\n");
        return 1;
    }

    /* initialize topics */
    emcute_topic_t t;
    t.name = EMCUTE_TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return 1;
    }

    while (1) {
        /* publish work state */
        int res = emcute_pub(&t, EMCUTE_MSG_WORK, sizeof(EMCUTE_MSG_WORK), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }
        
        xtimer_sleep(SLEEP_INTERVAL_SEC);
    }

    return 0;
}
