
#include <stdio.h>
#include <stdlib.h>
#include <net/sock.h>
#include <netinet/in.h>
#include <net/ipv4/addr.h>
#include <arpa/inet.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <timex.h>
#include <stdatomic.h>
#include <unistd.h>
#include <time_units.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/gcoap.h>
#include <stdbool.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <msg.h>
#include <net/sock/util.h>
#include <mbox.h>
#include <stddef.h>
#include <sys/socket.h>
#include <ztimer.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

static evtimer_msg_t evtimer;
static sock_tcp_t sock;
static sock_tcp_ep_t remote;

void publish_state(void) {
    char payload[] = MQTT_PAYLOAD;
    sock_tcp_send(&sock, payload, sizeof(payload), 0);
}

void connect_to_broker(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;

    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating TCP socket");
        return;
    }

    puts("Connected to MQTT broker");
}

void disconnect_from_broker(void) {
    sock_tcp_close(&sock);
    puts("Disconnected from MQTT broker");
}

void periodic_publish(void) {
    evtimer_add_msg(&evtimer, &(evtimer_msg_event_t){ .event = { .offset = 5 * US_PER_SEC } }, thread_getpid());
}

int main(void) {
    evtimer_init_msg(&evtimer);

    connect_to_broker();

    periodic_publish();

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        if (msg.type == EVTIMER_MSG_TYPE) {
            publish_state();
            periodic_publish();
        }
    }

    disconnect_from_broker();

    return 0;
}
