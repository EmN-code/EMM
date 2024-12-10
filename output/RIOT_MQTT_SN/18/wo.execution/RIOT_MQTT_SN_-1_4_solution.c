#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "arpa/inet.h"  // For inet_pton function

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define STATE_MSG "work"
#define INTERVAL (5U * US_PER_SEC)

static emcute_topic_t topic;
static sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };

void publish_state(void) {
    int res = inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);
    if (res != 1) {
        puts("ERROR: invalid IP address");
        return;
    }

    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Couldn't connect to the broker");
        return;
    }

    topic.name = MQTT_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Couldn't register topic");
        return;
    }

    while (1) {
        if (emcute_pub(&topic, STATE_MSG, strlen(STATE_MSG), 0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            puts("Published state message");
        }
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }
}

int main(void) {
    msg_t queue[8];
    msg_init_queue(queue, 8);
    publish_state();
    return 0;
}