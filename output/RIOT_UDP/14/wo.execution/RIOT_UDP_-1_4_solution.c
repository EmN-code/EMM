#include <mutex.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/af.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <esp_system.h>
#include <arpa/inet.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5000U * 1000U)  // in microseconds

int main(void) {
    /* Create a UDP socket */
    sock_udp_ep_t server = { .family = AF_INET,
                             .port = SERVER_PORT };

    inet_pton(AF_INET, SERVER_ADDR, &server.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return -1;
    }

    const char *message = "Device state: work";
    while (1) {
        if (sock_udp_send(&sock, message, strlen(message), &server) < 0) {
            printf("Error sending packet\n");
        }
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }

    sock_udp_close(&sock);
    return 0;
}