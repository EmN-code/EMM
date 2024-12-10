#include "netinet/in.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "mma8x5x_regs.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"

#define BROKER_PORT 1883
#define BROKER_ADDRESS "47.102.103.1"
#define PUBLISH_INTERVAL 5

static emcute_topic_t topic;
static sock_udp_ep_t gw;

void publish_state(void) {
    const char *state = "work";
    while (1) {
        if (emcute_pub(&topic, state, sizeof(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish message\n");
        }
        xtimer_sleep(PUBLISH_INTERVAL);
        state = (state[0] == 'w') ? "not work" : "work";
    }
}

int main(void) {
    (void)puts;  // Use puts to avoid removing the include

    /* set the gateway address */
    gw.family = AF_INET;
    gw.port = BROKER_PORT;
    sock_addr_in_init(&gw.addr, htons(INET_ADDRSTRLEN), inet_addr(BROKER_ADDRESS));

    /* initialize the gateway and topic */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect gateway\n");
        return 1;
    }

    topic.name = "state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic\n");
        return 1;
    }

    /* start publishing */
    publish_state();

    return 0;
}