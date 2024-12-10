#include <xtimer.h>
#include <net/sock/udp.h>

#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server_addr = { .family = AF_INET, .port = 20001 };
 
    /* Set server address */
    if (inet_pton(AF_INET, "47.102.103.1", &server_addr.addr.ipv4) != 1) {
        puts("Error: unable to parse server address");
        return 1;
    }

    /* Create a sock_udp struct and open it to enable sending */
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    char buf[] = "work"; // Change this to "not work" when necessary.

    while (1) {
        if (sock_udp_send(&sock, buf, sizeof(buf), &server_addr) < 0) {
            puts("Error: unable to send message");
        }
        xtimer_sleep(INTERVAL);
    }

    /* Close the socket (though in this loop, it will never reach here) */
    sock_udp_close(&sock);
    return 0;
}