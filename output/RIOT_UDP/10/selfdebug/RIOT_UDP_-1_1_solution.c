#include <xtimer.h>
#include <net/udp.h>
#include <net/af.h>
#include <string.h>

#define SERVER_PORT 20001
#define CLIENT_PORT 12345  // Arbitrary port for the client
#define SEND_INTERVAL (5U * US_PER_SEC)  // Send interval in microseconds (5 seconds)

int main(void) {
    printf("Starting UDP client example for ESP32\n");

    /* server address */
    ipv6_addr_t server_addr;
    // Convert the server address from IPv4 to IPv6-compatible address
    if (ipv6_addr_from_str(&server_addr, "::ffff:47.102.103.1") == NULL) {
        printf("Error: unable to parse server address\n");
        return 1;
    }

    /* create a socket for UDP */
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    local.port = CLIENT_PORT;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return 1;
    }
    
    /* prepare for sending data */
    sock_udp_ep_t remote = {.family = AF_INET6, .port = SERVER_PORT};
    memcpy(&remote.addr.ipv6, &server_addr, sizeof(server_addr));

    char *device_state = "work";  // or "not work"

    while (1) {
        /* send the device state */
        if (sock_udp_send(&sock, device_state, strlen(device_state), &remote) < 0) {
            printf("Error sending message\n");
        } else {
            printf("Message sent: %s\n", device_state);
        }
        /* wait for specified interval */
        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}