
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <xtimer.h>
#include <ztimer.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <timex.h>
#include <stdatomic.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <net/ipv6/addr.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <net/gcoap.h>
#include <time_units.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work or not work"
#define PUBLISH_INTERVAL (5 * US_PER_SEC)

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .port = BROKER_PORT };

static void publish_state(void) {
    const char *state_msg = STATE_MESSAGE;
    size_t msg_len = strlen(state_msg);
    unsigned flags = 0;

    int res = emcute_pub(&topic, state_msg, msg_len, flags);
    if (res != EMCUTE_OK) {
        printf("Failed to publish state: %d\n", res);
    } else {
        printf("State published: %s\n", state_msg);
    }
}

static void connect_to_broker(void) {
    int res = inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);
    if (res <= 0) {
        printf("Invalid broker IP address\n");
        return;
    }

    gw.family = AF_INET;
    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to broker: %d\n", res);
        return;
    }

    printf("Connected to broker at %s:%d\n", BROKER_IP, BROKER_PORT);

    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return;
    }

    printf("Topic registered: %s\n", TOPIC_NAME);
}

int main(void) {
    connect_to_broker();

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_USEC, PUBLISH_INTERVAL);
    }

    return 0;
}
