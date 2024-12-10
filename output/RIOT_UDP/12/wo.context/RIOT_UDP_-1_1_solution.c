#include "udp.h"
#include "thread.h"
#include "timex.h"
#include "ztimer.h"
#include <stdio.h>
#include <string.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001

static void *udp_sender_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &server.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return NULL;
    }

    char msg[] = "work";
    while (1) {
        if (sock_udp_send(&sock, msg, strlen(msg), &server) < 0) {
            printf("Error sending message\n");
        } else {
            printf("Message sent\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, udp_sender_thread, NULL, "udp_sender");
    return 0;
}