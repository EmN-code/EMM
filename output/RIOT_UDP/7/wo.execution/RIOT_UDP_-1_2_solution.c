#include "thread.h"
#include "event/thread.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/af.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT (20001)
#define BUFFER_SIZE (64)
#define INTERVAL (5U * 1000000U) // 5 seconds

char stack[THREAD_STACKSIZE_MAIN];
static sock_udp_ep_t server;

void *udp_send_thread(void *arg) {
    (void)arg;
    sock_udp_t sock;
    char buffer[BUFFER_SIZE];
    int res;

    /* Create the UDP socket */
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        return NULL;
    }

    /* Initialize the server endpoint */
    server.family = AF_INET;
    server.netif = SOCK_ADDR_ANY_NETIF;
    server.port = SERVER_PORT;
    res = sock_udp_str2ep(&server, SERVER_ADDR);
    if (res < 0) {
        return NULL;
    }

    while (1) {
        snprintf(buffer, BUFFER_SIZE, "work"); // for demonstration
        res = sock_udp_send(&sock, buffer, strlen(buffer), &server);

        /* Check if the message was sent successfully */
        if (res < 0) {
            sock_udp_close(&sock);
            return NULL;
        }

        /* Wait for the defined interval before sending the next message */
        thread_sleep_for(INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                  udp_send_thread, NULL, "udp_send_thread");
    return 0;
}