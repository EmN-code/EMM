#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <net/emcute.h>
#include <msg.h>
#include <stdio.h>
#include <stdlib.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state/work"
#define PUB_INTERVAL (5U)
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static void* emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, BROKER_IP);
    return NULL;
}

int main(void)
{
    msg_init_queue(queue, 8);
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    emcute_conn(BROKER_IP, BROKER_PORT, 0);

    while (1) {
        char *message = "work"; // You can change the state to "not work" if needed
        emcute_pub(TOPIC, message, strlen(message), EMCUTE_QOS_0);
        printf("Published: %s\n", message);

        sleep(PUB_INTERVAL);
    }

    return 0;
}