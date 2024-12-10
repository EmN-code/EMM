#include <mutex>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <kernel.h>
#include <msg.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC) // 5 seconds

static sock_udp_ep_t broker_endpoint;

void *publisher_thread(void *arg) {
    (void)arg;
    msg_t msg;
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    while (1) {
        const char *message = "work"; // State to publish
        ssize_t res = sock_udp_send(&sock, message, strlen(message), &broker_endpoint);
        if (res < 0) {
            printf("Error sending message\n");
        } else {
            printf("Message sent: %s\n", message);
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }
    sock_udp_close(&sock);
    return NULL;
}

int main(void) {
    // Setup the broker endpoint
    broker_endpoint.family = AF_INET;
    broker_endpoint.netif = SOCK_ADDR_ANY;
    broker_endpoint.port = BROKER_PORT;

    // Convert broker IP from string to binary
    ipv6_addr_from_str((ipv6_addr_t *)&broker_endpoint.addr.ipv6, BROKER_IP);

    // Create the publisher thread
    char publisher_stack[THREAD_STACKSIZE_MAIN];
    thread_create(publisher_stack, sizeof(publisher_stack), THREAD_PRIORITY_MAIN-1,
                  THREAD_CREATE_STACKTEST, publisher_thread, NULL, "publisher");

    // The main thread can do other tasks in parallel
    // For demonstration, we'll just let it sleep
    while (1) {
        xtimer_sleep(60);
    }

    return 0;
}