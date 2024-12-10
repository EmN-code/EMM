#include "thread.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define MESSAGE_INTERVAL (5U)

static char stack[THREAD_STACKSIZE_MAIN];

void *send_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    inet_pton(AF_INET6, SERVER_ADDR, &remote.addr.ipv6);

    char data[] = "work";

    while (1) {
        sock_udp_send(NULL, data, sizeof(data), &remote);
        printf("Sent data to broker: %s\n", data);
        thread_yield_higher();
    }

    return NULL;
}

int main(void) {
    puts("MQTT over UDP demo\n");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  send_thread, NULL, "send_thread");

    /* Main thread just yields indefinitely */
    while (1) {
        thread_yield();
    }

    return 0;
}
