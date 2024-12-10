
#include "net/sock/udp.h"
#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include "net/ipv4/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_t sock;
static sock_udp_ep_t server = SOCK_IPV4_EP_ANY;

void *sender_thread(void *arg)
{
    (void)arg;

    while (1) {
        const char *message = "work";  // or "not work" based on device state
        sock_udp_send(&sock, message, strlen(message), &server);
        xtimer_usleep(SEND_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    // Initialize the server endpoint
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);
    server.port = SERVER_PORT;
    server.family = AF_INET;

    // Create and initialize the UDP socket
    if (sock_udp_create(&sock, NULL, &server, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Start the sender thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");

    return 0;
}
