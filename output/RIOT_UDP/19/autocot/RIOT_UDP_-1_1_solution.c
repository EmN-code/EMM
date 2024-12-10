#include <xtimer.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define INTERVAL (5U * 1000000U)  // interval to send the message, in microseconds

static void send_device_state(sock_udp_t *sock, const sock_udp_ep_t *remote);

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET, .netmask_len = 0, .port = SERVER_PORT };
    
    // Convert the IP address from string to network format
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IP);
    
    // Open the UDP connection
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }
    
    while (1) {
        send_device_state(&sock, &remote);
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    // Close the socket (not reachable in this code)
    sock_udp_close(&sock);
    return 0;
}

static void send_device_state(sock_udp_t *sock, const sock_udp_ep_t *remote) {
    printf("Sending device state: %s\n", MESSAGE);
    
    if (sock_udp_send(sock, MESSAGE, strlen(MESSAGE), remote) < 0) {
        printf("Error sending UDP message\n");
    } else {
        printf("Message sent successfully\n");
    }
}