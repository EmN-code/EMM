#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <net/mqttsn.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "state"
#define QOS 0
#define MESSAGE "work"
#define PUB_INTERVAL 5

static sock_udp_ep_t gw = { .family = AF_INET6, .port = SERVER_PORT };

void *publish_thread(void *arg) {
    /* Buffer for the message */
    char message[64];

    /* Set message content*/
    snprintf(message, sizeof(message), MESSAGE);

    while (1) {
        /* Publish message to the broker */
        int res = mqttsn_pub(CLIENT_ID, strlen(CLIENT_ID), &gw, TOPIC, strlen(TOPIC), message, strlen(message), QOS);
        if (res < 0) {
            printf("Failed to publish message\n");
        }
        else {
            printf("Published: %s\n", message);
        }

        /* Wait for the defined interval */
        sleep(PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    /* Initialize gateway address structure */
    if (inet_pton(AF_INET6, SERVER_ADDR, &gw.addr.ipv6) != 1) {
        printf("Failed to parse server address\n");
        return 1;
    }

    /* Creating thread for publishing */
    static char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  publish_thread, NULL, "publish_thread");

    return 0;
}