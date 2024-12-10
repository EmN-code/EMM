#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/af.h>
#include <kernel_defines.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define TIMER_INTERVAL (5U * US_PER_SEC)  // 5 seconds in microseconds

static void *udp_sender(void *arg) {
    (void)arg;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);

    char message[] = "work"; // Change state accordingly
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);
    
    while (1) {
        sock_udp_send(&sock, message, sizeof(message), &remote);
        printf("Sent message: %s\n", message);
        // Alternate message to simulate device state
        strcpy(message, strcmp(message, "work") == 0 ? "not work" : "work");
        xtimer_sleep(TIMER_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    printf("UDP client example
");
    udp_sender(NULL);
    return 0;
}