
#include "mutex.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "netinet/in.h"
#include "dht_params.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "esp_system.h"
#include "net/sock/util.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

void send_state(void) {
    const char *state = DEVICE_STATE;
    ssize_t res = sock_udp_send(&sock, state, strlen(state), &remote);
    if (res < 0) {
        printf("Failed to send UDP message: %d\n", (int)res);
    } else {
        printf("Sent device state: %s\n", state);
    }
}

int main(void) {
    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid server IP address\n");
        return 1;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return 1;
    }

    while (1) {
        send_state();
        // Sleep for 5 seconds
        vTaskDelay(pdMS_TO_TICKS(5000));
    }

    // Close the socket (this will never be reached in this example)
    sock_udp_close(&sock);

    return 0;
}
