#include "ztimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "string.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "dht_params.h"
#include "arpa/inet.h"

#define SLEEP_INTERVAL 5
#define MQTT_SN_PORT   1883
#define SERVER_ADDRESS "47.102.103.1"

static emcute_topic_t topic;
static char *message = "work";

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_PORT };
    if (inet_pton(AF_INET, SERVER_ADDRESS, &gw.addr) != 1) {
        printf("Error: unable to parse IPv4 address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to broker\n");
        return 1;
    }

    topic.name = "state";
    emcute_reg(&topic);

    while (1) {
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish message\n");
        }
        ztimer_sleep(ZTIMER_SEC, SLEEP_INTERVAL);
    }

    emcute_discon();
    return 0;
}