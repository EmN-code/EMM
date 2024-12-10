#include <netinet/in.h>
#include <net/sock/udp.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

void send_device_state(sock_udp_ep_t *remote) {
    char *device_state = "work";  // Change this as needed ("work" or "not work")
    ssize_t res = sock_udp_send(NULL, device_state, sizeof(device_state), remote);
    if (res <= 0) {
        puts("Error sending UDP packet");
    }
    else {
        puts("Device state sent");
    }
}

int main(void) {
    sock_udp_ep_t remote;
    uint8_t addr[4];

    // Convert IP address string into byte array
    if (inet_pton(AF_INET, SERVER_IP, addr) != 1) {
        puts("Error: could not parse IP address");
        return -1;
    }

    // Setup remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    memcpy(&remote.addr.ipv4, addr, sizeof(addr));

    while (1) {
        send_device_state(&remote);
        xtimer_sleep(SEND_INTERVAL / 1000000U);  // Convert microseconds back to seconds for sleep
    }

    return 0;
}