#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/sock.h>
#include <stdio.h>
#include <msg.h>
#include "kernel_types.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_STATE_TOPIC "device/state"
#define PUB_INTERVAL (5U * US_PER_SEC)

static char state_msg[20];

void mqtt_publish(void) {
    sock_udp_ep_t broker_ep = {
        .family = AF_INET6,
        .port = BROKER_PORT
    };
    inet_pton(AF_INET6, BROKER_IP, &broker_ep.addr.ipv6);

    sock_udp_t sock;
    sock_udp_create(&sock, &broker_ep, NULL, 0);

    msg_t queue[8];
    msg_init_queue(queue, 8);

    while (1) {
        snprintf(state_msg, sizeof(state_msg), "work or not work");
        sock_udp_send(&sock, state_msg, sizeof(state_msg), &broker_ep);
        printf("Published state: %s\n", state_msg);
        xtimer_sleep(PUB_INTERVAL);
    }
}

int main(void) {
    puts("MQTT publisher using plain UDP started");
    mqtt_publish();
    return 0;
}