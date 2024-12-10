#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <msg.h>

#define SENDER_PRIO (THREAD_PRIORITY_MAIN + 1)
#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define INTERVAL (1000 * US_PER_MS) // sending interval in microseconds

static kernel_pid_t sender_pid;
static char sender_stack[STACKSIZE];

static void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1; // arbitrary message type

    while (1) {
        xtimer_usleep(INTERVAL);
        printf("Sending message from sender\n");
        msg_send(&msg, thread_getpid());
    }

    return NULL;
}

int main(void) {
    puts("Message Box Example");
    sender_pid = thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0, sender, NULL, "sender");
    msg_t msg_queue[4];
    msg_init_queue(msg_queue, 4);
    msg_t msg;

    while (1) {
        if (msg_receive(&msg) > 0) {
            printf("Received message of type: %d\n", msg.type);
        }
    }
    return 0;
}