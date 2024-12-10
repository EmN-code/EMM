
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <timex.h>
#include <xtimer.h>
#include <stdbool.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <msg.h>
#include <thread.h>
#include <kernel_defines.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <dht.h>
#include <dht_params.h>
#include <event/thread.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <net/gcoap.h>
#include <stddef.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <time_units.h>
#include <architecture.h>
#include <mbox.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

static void publish_state(void) {
    char payload[PAYLOAD_LEN];
    memcpy(payload, PAYLOAD, PAYLOAD_LEN);

    sock_udp_send(&sock, payload, PAYLOAD_LEN, &remote);
}

static void timer_callback(void *arg) {
    (void)arg;
    publish_state();
    xtimer_set(&timer, INTERVAL);
}

static xtimer_t timer;

int main(void) {
    local.port = 0;
    remote.family = AF_INET;
    remote.port = htons(BROKER_PORT);

    if (inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid broker IP address");
        return 1;
    }

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error: failed to create UDP socket");
        return 1;
    }

    xtimer_init(&timer);
    xtimer_set(&timer, INTERVAL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        timer_callback(NULL);
    }

    return 0;
}
