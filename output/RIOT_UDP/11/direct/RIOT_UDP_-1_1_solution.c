#include <string.h>
#include <stdio.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <net/sock.h>
#include <xtimer.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define BUFFER_SIZE 64
#define SEND_INTERVAL (5U * US_PER_SEC)

int main(void)
{
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };

    /* Convert IP address from string to a network byte order binary form */
    if (ipv6_addr_from_str((ipv6_addr_t *)&server.addr, SERVER_ADDR) == NULL) {
        puts("Error: unable to parse IPv6 address");
        return 1;
    }

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    const char *device_state = "work";

    while (1) {
        /* Create the message to be sent */
        snprintf(buffer, sizeof(buffer), "Device state: %s", device_state);

        /* Send the data to the server */
        if (sock_udp_send(&sock, buffer, strlen(buffer), &server) < 0) {
            puts("Error: unable to send message");
        } else {
            puts("Message sent successfully");
        }

        /* Wait for the send interval */
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
    }

    return 0;
}