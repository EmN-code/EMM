#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread.h>
#include <time_units.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <msg.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL (5U * US_PER_SEC) // Sleep interval of 5 seconds

void *send_state_thread(void *arg) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = SERVER_PORT
    };
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        const char *state = "work"; // Assume device is working; change logic as needed
        sock_udp_send(&sock, state, strlen(state), &server);
        ztimer_sleep(ZTIMER_SEC, 5); // Sleep for 5 seconds
    }

    sock_udp_close(&sock);
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, send_state_thread, NULL, "send_state");
    return 0;
}
