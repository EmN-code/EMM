#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <netinet/in.h>

#define SERVER_IP      "47.102.103.1"
#define SERVER_PORT    20001
#define MSG_INTERVAL   (5U * US_PER_SEC)

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    char *device_state_work = "work";
    char *device_state_not_work = "not work";
    char *current_state = device_state_work; // Set your initial state here
    
    while (1) {
        sock_udp_send(&sock, current_state, strlen(current_state), &server);
        xtimer_sleep(MSG_INTERVAL / US_PER_SEC);

        // Toggle device state for demonstration purposes
        if (strcmp(current_state, "work") == 0) {
            current_state = device_state_not_work;
        } else {
            current_state = device_state_work;
        }
    }
    
    return 0;
}