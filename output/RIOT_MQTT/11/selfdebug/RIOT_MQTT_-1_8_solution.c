#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread.h>
#include <xtimer.h>
#include <net/sock.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];

static void *publish_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    xtimer_ticks32_t last_wakeup = xtimer_now();

    if (inet_pton(AF_INET6, SERVER_ADDR, &remote.addr) <= 0) {
        puts("Error: unable to parse server address");
        return NULL;
    }

    while (1) {
        char *state = "work or not work";
        // Connecting code to the broker should be added here

        // Publish message (Simulating publication)
        printf("Publishing state: %s to broker at %s:%d\n", state, SERVER_ADDR, SERVER_PORT);

        // Wait for the next publish interval
        xtimer_periodic_wakeup(&last_wakeup, PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    puts("RIOT MQTT Client Example");

    // Creating a thread for publishing
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publish_thread, NULL, "publish_thread");

    return 0;
}