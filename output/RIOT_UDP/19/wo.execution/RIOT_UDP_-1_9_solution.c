#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "net/sock/udp.h"
#include "msg.h"

#define SERVER_PORT (20001)
#define SEND_INTERVAL (5U)

static sock_udp_ep_t remote;
static char message[] = "work";

void *send_thread(void *arg) {
    (void)arg;
    sock_udp_t sock;
    char response[128];
    ssize_t res;
    
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error in creating UDP socket");
        return NULL;
    }

    while (1) {
        res = sock_udp_send(&sock, message, sizeof(message), &remote);
        if (res < 0) {
            puts("Could not send message");
        } else {
            printf("Sent %ld bytes: %s\n", res, message);
        }

        xtimer_sleep(SEND_INTERVAL);
    }

    return NULL;
}

int main(void) {
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, send_thread, NULL, "send_thread");

    return 0;
}