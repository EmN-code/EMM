
#include <stdlib.h>
#include <net/emcute.h>
#include <msg.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <esp_system.h>
#include <time_units.h>
#include <xtimer.h>
#include <mutex.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <dht.h>
#include <esp_wifi.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>
#include <net/gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define DELAY_MS 5000

static sock_udp_t sock;
static sock_udp_ep_t remote;

void send_state(void) {
    const char *state = DEVICE_STATE;
    sock_udp_send(&sock, state, strlen(state), &remote);
}

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        send_state();
        xtimer_sleep(DELAY_MS / 1000);
    }

    sock_udp_close(&sock);
    return 0;
}
