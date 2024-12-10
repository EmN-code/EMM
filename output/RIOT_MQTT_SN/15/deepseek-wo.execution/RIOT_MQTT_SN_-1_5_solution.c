
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <event/thread.h>
#include <thread.h>
#include <mma8x5x_params.h>
#include <mutex.h>
#include <net/gcoap.h>
#include <stdio.h>
#include <dht_params.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stddef.h>
#include <net/sock/udp.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work"

static emcute_topic_t topic;

void publish_state(void) {
    const char *msg = MESSAGE;
    int res = emcute_pub(&topic, msg, strlen(msg), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing message: %d\n", res);
    } else {
        printf("Published: %s\n", msg);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .port = EMQX_SERVER_PORT };
    inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN gateway: %d\n", res);
        return 1;
    }

    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
