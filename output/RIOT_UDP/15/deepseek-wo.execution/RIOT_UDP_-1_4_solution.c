
#include <net/ipv6/addr.h>
#include <mutex.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <netinet/in.h>
#include <dht_params.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <esp_system.h>
#include <net/sock/util.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL_MS 5000

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    char buffer[256];

    // Convert server IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Invalid server IP address");
        return 1;
    }

    // Create a UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Failed to create UDP socket");
        return 1;
    }

    while (1) {
        // Prepare the message to send
        snprintf(buffer, sizeof(buffer), "Device state: %s", DEVICE_STATE);

        // Send the message to the server
        if (sock_udp_send(&sock, buffer, strlen(buffer), &remote) < 0) {
            puts("Failed to send message");
        } else {
            puts("Message sent successfully");
        }

        // Wait for 5 seconds before sending the next message
        ztimer_sleep(ZTIMER_MSEC, INTERVAL_MS);
    }

    // Close the socket (this part will never be reached in this example)
    sock_udp_close(&sock);

    return 0;
}
