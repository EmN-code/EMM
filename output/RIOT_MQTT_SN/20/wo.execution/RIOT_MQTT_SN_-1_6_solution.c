#include <xtimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdatomic.h>
#include <string.h> // Alternative for string operations within borders
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <arpa/inet.h>
#include <msg.h>
#include <time_units.h>
#include <event/thread.h>
#include <thread.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <net/ipv6/addr.h>
#include <sys/socket.h>

#define STACKSIZE (THREAD_STACKSIZE_MAIN / 2)
#define TOPIC_NAME "status/work"
#define EMCUTE_PORT 1883
#define EMCUTE_ID   "riot"
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static msg_t queue[8];
static kernel_pid_t emcute_pid;

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, 8);
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL; // The emcute_run function should block
}

int main(void) {
    // Initialize sockaddr_in6 structure correctly
    struct sockaddr_in6 remote = { 0 };
    inet_pton(AF_INET6, "::ffff:47.102.103.1", &remote.sin6_addr);
    remote.sin6_family = AF_INET6;
    remote.sin6_port = htons(EMCUTE_PORT);

    // Create emcute thread for MQTT-SN
    emcute_pid = thread_create(stack, sizeof(stack), EMCUTE_PRIO,
                               THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    // Prepare topic and message
    emcute_topic_t topic;
    strncpy((char *)topic.name, TOPIC_NAME, sizeof(topic.name));

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Could not obtain topic ID");
        return 1;
    }

    while (1) {
        char *message = "work"; // or "not work" depending on logic
        if (emcute_pub(&topic, message, strlen(message), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }
        xtimer_sleep(5);
    }
    return 0;
}