#include <kernel_defines.h>
#include <event/callback.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/af.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL 5 * US_PER_SEC

int main(void) {
    /* Define UDP socket, endpoint, buffer and other necessary variables */
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    char *data = "work or not work";
    uint8_t buf[64];
    sock_udp_t sock;

    /* Convert IP address from text to binary form */
    int res = inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);
    if (res <= 0) {
        puts("Error: Couldn't parse IP address");
        return 1;
    }

    /* Create the UDP socket */
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: Couldn't create UDP socket");
        return 1;
    }

    while (1) {
        /* Send the device state to the server */
        if (sock_udp_send(NULL, data, strlen(data), &server) < 0) {
            puts("Error: Couldn't send data");
        } else {
            puts("Data sent");
        }
        /* Wait for the specified interval */
        xtimer_sleep(5);
    }
 
    sock_udp_close(&sock);
    return 0; /* Should never be reached */
}