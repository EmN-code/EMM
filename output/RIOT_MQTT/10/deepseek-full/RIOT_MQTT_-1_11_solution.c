
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <evtimer_msg.h>
#include <thread.h>
#include <msg.h>
#include <xtimer.h>
#include <stdbool.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 1
#define TIMEOUT 10000L

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

static void connect_to_broker(void) {
    remote.family = AF_INET;
    remote.port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }
}

static void publish_state(void) {
    const char *payload = PAYLOAD;
    size_t payload_len = strlen(payload);

    if (sock_udp_send(&sock, payload, payload_len, &remote) < 0) {
        puts("Error sending UDP packet");
    } else {
        puts("State published");
    }
}

static void timer_callback(void *arg) {
    (void)arg;
    publish_state();
}

int main(void) {
    connect_to_broker();

    evtimer_msg_t evtimer;
    evtimer_msg_event_t event;

    evtimer_init_msg(&evtimer);
    event.event.offset = xtimer_ticks_from_usec(5 * 1000 * 1000);
    event.event.callback = timer_callback;

    while (1) {
        evtimer_add_msg(&evtimer, &event, thread_getpid());
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
