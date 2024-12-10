#include <unistd.h>        // for sleep function
#include <string.h>        // for memset
#include <stdio.h>         // for printf
#include <net/sock/udp.h>  // for sock_udp

/* Main function */
int main(void) {
    char buf[] = "work";  // device state
    sock_udp_ep_t server = { .family = AF_INET, .port = 20001 };
    inet_pton(AF_INET, "47.102.103.1", &server.addr.ipv4);
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        int res = sock_udp_send(&sock, buf, sizeof(buf), &server);
        if (res < 0) {
            printf("Error on sending: %d\n", res);
        }
        sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}