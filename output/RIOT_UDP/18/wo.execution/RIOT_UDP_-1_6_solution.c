#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "msg.h"
#include "ztimer.h"
#include "timex.h"
#include "net/sock/udp.h"
#include "stdatomic.h"
#include "thread.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define BUF_SIZE 128
#define SEND_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

static char message_stack[THREAD_STACKSIZE_MAIN];
static sock_udp_ep_t remote;

void *sender_thread(void *arg) {
    (void)arg;
    sock_udp_t sock;
    char buffer[BUF_SIZE];

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_ADDR);

    while (1) {
        snprintf(buffer, BUF_SIZE, "Device state: %s", "work");
        if (sock_udp_send(&sock, buffer, strlen(buffer), &remote) < 0) {
            puts("Error sending message");
        } else {
            puts("Message sent");
        }
        ztimer_sleep(ZTIMER_MSEC, SEND_INTERVAL);
    }

    return NULL;
}

int main(void) {
    puts("ESP32 UDP client example");

    thread_create(message_stack, sizeof(message_stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender_thread");

    return 0;
}