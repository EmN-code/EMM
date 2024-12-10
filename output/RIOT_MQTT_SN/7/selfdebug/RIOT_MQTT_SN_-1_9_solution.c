#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/ipv6/addr.h>
#include "net/emcute.h"
#include "net/sock/udp.h"

#define EMCUTE_ID "esp_node"

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t _msg_q[8];

#define STATE_TOPIC_ID 1234

static const char *state_work = "work";
static const char *state_not_work = "not work";

static emcute_topic_t state_topic = {
    .name = "device/state",
    .id = STATE_TOPIC_ID
};

static sock_udp_ep_t gw = {
    .family = AF_INET6,
    .port = 1883
};

int pub_state(bool working) {
    const char *state = working ? state_work : state_not_work;
    return emcute_pub(&state_topic, state, strlen(state), EMCUTE_QOS_0);
}

int main(void) {
    /* message queue
     * must call msg_init_queue before using msg_send or msg_receive
     */
    msg_init_queue(_msg_q, sizeof(_msg_q) / sizeof(msg_t));

    /*setup address to gateway */
    char *gw_addr_str = "47.102.103.1";
    inet_pton(AF_INET6, gw_addr_str, &gw.addr);

    printf("Connecting to gateway %s...