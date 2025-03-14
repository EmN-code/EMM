
#include "mutex.h"
#include "net/sock.h"
#include "net/emcute.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "esp_system.h"
#include "arpa/inet.h"
#include "dht_params.h"
#include "sys/socket.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

static void send_device_state(const char *state) {
    ssize_t res = sock_udp_send(&sock, state, strlen(state), &remote);
    if (res < 0) {
        printf("Failed to send message: %d\n", (int)res);
    } else {
        printf("Sent device state: %s\n", state);
    }
}

int main(void) {
    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Create the UDP socket
    int res = sock_udp_create(&sock, &local, &remote, 0);
    if (res < 0) {
        printf("Failed to create UDP socket: %d\n", res);
        return 1;
    }

    // Main loop to send device state every 5 seconds
    while (1) {
        send_device_state(DEVICE_STATE_WORK);
        xtimer_usleep(SEND_INTERVAL);
    }

    // Close the socket (this will never be reached in this example)
    sock_udp_close(&sock);

    return 0;
}
