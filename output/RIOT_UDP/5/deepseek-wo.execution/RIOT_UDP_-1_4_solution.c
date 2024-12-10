
#include <arpa/inet.h>
#include <esp_system.h>
#include <mutex.h>
#include <net/gcoap.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <netinet/in.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

void send_device_state(void) {
    const char *state = DEVICE_STATE;
    sock_udp_send(&sock, state, strlen(state), &remote);
}

int main(void) {
    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Create a UDP socket
    sock_udp_create(&sock, &local, &remote, 0);

    while (1) {
        send_device_state();
        xtimer_sleep(5);
    }

    // Close the socket
    sock_udp_close(&sock);

    return 0;
}
