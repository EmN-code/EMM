
#include "esp_system.h"
#include "mutex.h"
#include "dht_params.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/sock.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SLEEP_TIME (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

void init_udp_socket(void) {
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        printf("Error creating UDP socket\n");
        return;
    }
}

void send_device_state(void) {
    const char *state = DEVICE_STATE;
    if (sock_udp_send(&sock, state, strlen(state), &remote) < 0) {
        printf("Error sending UDP packet\n");
    }
}

int main(void) {
    init_udp_socket();

    while (1) {
        send_device_state();
        ztimer_sleep(ZTIMER_USEC, SLEEP_TIME);
    }

    return 0;
}
