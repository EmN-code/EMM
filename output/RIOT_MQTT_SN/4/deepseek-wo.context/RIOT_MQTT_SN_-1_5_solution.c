
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ADDR "47.102.103.1"
#define TOPIC "state"
#define MSG_SIZE 16
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t topic;

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_t sock;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    while (1) {
        if (sock_udp_recv(&sock, NULL, 0, EMCUTE_PORT, &gw) < 0) {
            puts("Error receiving from socket");
            continue;
        }

        emcute_run(&sock, &gw);
    }

    return NULL;
}

int main(void)
{
    // Initialize network stack
    if (net_init() < 0) {
        puts("Error initializing network stack");
        return 1;
    }

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, EMCUTE_ADDR, &gw.addr.ipv4) != 1) {
        puts("Error converting IP address");
        return 1;
    }

    // Start the EMCUTE thread
    thread_create(emcute_stack, sizeof(emcute_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    // Register the topic
    topic.name = TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    // Publish state every 5 seconds
    char msg[MSG_SIZE];
    while (1) {
        strcpy(msg, "work");
        if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
            puts("Error publishing message");
        }

        xtimer_usleep(INTERVAL);
    }

    return 0;
}
