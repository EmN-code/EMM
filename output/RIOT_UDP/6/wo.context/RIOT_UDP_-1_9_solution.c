#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MSG_INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);
    
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    char *msg_work = "device_status: work";
    char *msg_not_work = "device_status: not work";

    while (1) {
        // Simulating device check here
        int device_status = 1; // 1 for work, 0 for not work
        char *msg = device_status ? msg_work : msg_not_work;
        
        ssize_t res = sock_udp_send(&sock, msg, strlen(msg), &server);
        if (res <= 0) {
            printf("Error sending: %ld\n", (long)res);
        }
        xtimer_sleep(MSG_INTERVAL);
    }

    sock_udp_close(&sock);

    return 0;
}