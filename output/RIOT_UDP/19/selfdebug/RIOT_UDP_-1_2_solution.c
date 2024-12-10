#include "thread.h"
#include "net/ipv6/addr.h"
#include "msg.h"
#include "sock/udp.h"
#include "ztimer.h"

#define SERVER_PORT 20001
#define SERVER_ADDR "47.102.103.1"
#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static sock_udp_ep_t server = {
    .family = AF_INET,
    .port = SERVER_PORT
};

static void udp_send_task(void) {
    char *state = "work or not work";
    while (1) {
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
        sock_udp_send(NULL, state, strlen(state) + 1, &server);
    }
}

int main(void) {
    server.addr.ipv4.u32[0] = inet_addr(SERVER_ADDR);
    
    // Create a new thread for sending UDP messages
    thread_create(
        stack,
        sizeof(stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        udp_send_task,
        NULL,
        "udp_send"
    );

    // Keep the main function running indefinitely
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}