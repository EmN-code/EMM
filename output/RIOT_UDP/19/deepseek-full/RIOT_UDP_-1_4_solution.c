
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <esp_system.h>
#include <thread.h>
#include <mutex.h>
#include <event/thread.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <dht_params.h>
#include <sys/socket.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define SEND_INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

static void send_device_state(void)
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%s", DEVICE_STATE_MSG);

    if (sock_udp_send(&sock, buffer, strlen(buffer), &remote) < 0) {
        puts("Failed to send device state");
    } else {
        puts("Device state sent successfully");
    }
}

int main(void)
{
    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Invalid server IP address");
        return 1;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Failed to create UDP socket");
        return 1;
    }

    while (1) {
        send_device_state();
        usleep(SEND_INTERVAL);
    }

    // Close the socket
    sock_udp_close(&sock);

    return 0;
}
