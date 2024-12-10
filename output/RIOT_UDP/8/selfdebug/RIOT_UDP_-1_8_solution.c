#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <string.h>

#define SERVER_ADDRESS "47.102.103.1"
#define SERVER_PORT 20001
#define STACK_SIZE THREAD_STACKSIZE_DEFAULT
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char network_thread_stack[STACK_SIZE]; // Correct the variable name to match your declaration

void *network_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDRESS, &server.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        const char *msg = "Device is working"; // or "Device is not working" depending on state
        sock_udp_send(&sock, msg, strlen(msg), &server);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    puts("RIOT UDP client example started");

    thread_create(network_thread_stack, sizeof(network_thread_stack), PRIORITY,
                  THREAD_CREATE_STACKTEST, network_thread, NULL, "network");

    return 0;
}