#include <xtimer.h>
#include <timex.h>
#include <net/sock/udp.h>
#include <net/af.h>

#define SERVER_IP   "47.102.103.1"
#define SERVER_PORT 20001
#define MSG_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
    inet_pton(AF_INET6, SERVER_IP, &server.addr.ipv6);
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    char send_data[] = "work";
    xtimer_ticks32_t last_wakeup = xtimer_now();
    
    // Open the UDP socket
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error: Could not open socket");
        return -1;
    }

    while (1) {
        if (sock_udp_send(&sock, send_data, sizeof(send_data), &server) < 0) {
            puts("Error: Could not send data");
        } else {
            puts("Data sent successfully");
        }
        xtimer_periodic_wakeup(&last_wakeup, MSG_INTERVAL);
    }
    return 0;
}