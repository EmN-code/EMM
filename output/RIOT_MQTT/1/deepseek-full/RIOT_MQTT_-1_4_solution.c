
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <net/ipv4/addr.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <thread.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <stdatomic.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <dht_params.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

static void *sender_thread(void *arg)
{
    (void)arg;
    char payload[] = PAYLOAD;
    while (1) {
        if (sock_udp_send(&sock, payload, sizeof(payload) - 1, &remote) < 0) {
            puts("Error sending message");
        } else {
            puts("Message sent");
        }
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }
    return NULL;
}

int main(void)
{
    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = EMQX_SERVER_PORT;
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error parsing server IP address");
        return 1;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Start the sender thread
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    return 0;
}
