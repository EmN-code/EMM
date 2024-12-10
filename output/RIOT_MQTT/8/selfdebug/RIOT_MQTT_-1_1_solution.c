// Include RIOT-related and MQTT necessary headers
#include "thread.h"
#include "timex.h"
#include "msg.h"
#include "net/emcute.h"
#include "net/ipv6/netif.h"
#include "net/sock/udp.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL_SEC (5U)
#define TOPIC "device/state"
#define WORK_MESSAGE "work"
#define NOT_WORK_MESSAGE "not work"

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

// Function to establish network connection
static int connect_to_broker(sock_udp_t *sock) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = BROKER_PORT };

    // Convert IP address to binary form
    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, BROKER_IP) == NULL) {
        printf("Error: unable to parse broker address\n");
        return -1;
    }

    // Create UDP connection
    if (sock_udp_create(sock, NULL, &remote, 0) < 0) {
        printf("Error: cannot create UDP connection\n");
        return -1;
    }

    return 0;
}

// Publish state function
type state_publish(sock_udp_t *sock, int work_state) {
    char *message = work_state ? WORK_MESSAGE : NOT_WORK_MESSAGE;
    int message_len = strlen(message);

    // Publish the message
    if (sock_udp_send(sock, message, message_len, NULL) < 0) {
        printf("Error: unable to send message\n");
        return -1;
    }

    printf("Published: %s\n", message);
    return 0;
}

// Main application function
int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    // Initialize UDP socket
    sock_udp_t sock;

    // Try connecting to broker
    if (connect_to_broker(&sock) != 0) {
        printf("Failed to connect to broker\n");
        return 1;
    }

    while (1) {
        state_publish(&sock, 1); // Simulate "work" state
        xtimer_sleep(PUBLISH_INTERVAL_SEC);
        state_publish(&sock, 0); // Simulate "not work" state
        xtimer_sleep(PUBLISH_INTERVAL_SEC);
    }

    return 0;
}