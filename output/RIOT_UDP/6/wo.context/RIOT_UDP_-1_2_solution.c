#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <stdio.h>
#include <string.h>

#define SERVER_PORT (20001)
#define SEND_INTERVAL (5U * US_PER_SEC)

static const char *server_ip = "47.102.103.1";

int main(void)
{
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    char data[] = "work";

    // Parse the server IP address (IPv4)
    if (ipv4_addr_from_str((ipv4_addr_t *)&server.addr, server_ip) == NULL) {
        puts("Error: unable to decode server address");
        return 1;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    while (1) {
        // Send the data to the server
        if (sock_udp_send(&sock, data, strlen(data), &server) < 0) {
            puts("Error: unable to send data");
        } else {
            puts("State sent: work");
        }
        
        // Wait for the specified interval
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
    }

    // Close the socket (note: this line is never actually reached because of the loop)
    // sock_udp_close(&sock);

    return 0;
}