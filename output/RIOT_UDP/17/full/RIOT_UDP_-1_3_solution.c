#include <xtimer.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <stdio.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC) // Send every 5 seconds

int main(void)
{
    sock_udp_ep_t server = { .family = AF_INET, .port = UDP_SERVER_PORT };
    int res;
    char *state_message = "work";
    uint8_t buf[128];
    size_t len;

    // Convert IP address
    res = inet_pton(AF_INET, UDP_SERVER_IP, &server.addr.ipv4);
    if (res != 1) {
        puts("Error: unable to parse IPv4 address");
        return 1;
    }

    sock_udp_t sock;
    res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res != 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    puts("Sending device state every 5 seconds...");
    while (1) {
        len = snprintf((char *)buf, sizeof(buf), "%s", state_message);

        // Send the message to the server
        res = sock_udp_send(&sock, buf, len, &server);
        if (res < 0) {
            printf("Error sending message: %d\n", res);
        } else {
            printf("Sent: %s\n", state_message);
        }

        // Wait for the next interval
        xtimer_sleep(SEND_INTERVAL / US_PER_SEC);
    }

    return 0;
}