#include <udp.h>
#include <thread.h>
#include <stdio.h>
#include <string.h>

#define SERVER_ADDR  "47.102.103.1"
#define SERVER_PORT  20001
#define SEND_INTERVAL (5U)

static sock_udp_ep_t server;

void *send_thread(void *arg) {
    char *device_state = "work";
    sock_udp_t sock;

    (void)arg;

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    while (1) {
        ssize_t res = sock_udp_send(&sock, device_state, strlen(device_state), &server);
        if (res < 0) {
            puts("Error sending UDP packet");
        } else {
            printf("Sent %s to %s:%u\n", device_state, SERVER_ADDR, SERVER_PORT);
        }
        xtimer_sleep(SEND_INTERVAL);
    }
    return NULL;
}

int main(void) {
    puts("Starting UDP client to send device state...");

    server.family = AF_INET6; // Use IPv6 family
    ipv6_addr_from_str((ipv6_addr_t *)&server.addr, SERVER_ADDR);
    server.port = SERVER_PORT;

    static char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, send_thread, NULL, "send_thread");

    return 0;
}