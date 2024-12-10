#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <thread.h>
#include <time_units.h>
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "msg.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 2)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 3)

typedef struct msg msg_t;

static char stack_sender[THREAD_STACKSIZE_MAIN];
static char stack_receiver[THREAD_STACKSIZE_MAIN];

void *sender_thread(void *arg) {
    (void) arg;
    msg_t msg;
    msg.content.value = 42;
    msg.type = 1;
    printf("Sender: Sending message with value %lu\n", (unsigned long)msg.content.value);
    msg_send(&msg, thread_getpid());
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(&msg, 1);
    if (msg_receive(&msg) == 1) {
        printf("Receiver: Received message with value %lu\n", (unsigned long)msg.content.value);
    }
    return NULL;
}

int main(void) {
    thread_create(stack_sender, sizeof(stack_sender), THREAD_PRIORITY_SENDER, 0, sender_thread, NULL, "sender");
    thread_create(stack_receiver, sizeof(stack_receiver), THREAD_PRIORITY_RECEIVER, 0, receiver_thread, NULL, "receiver");
    while (1) {
        sleep(1);
    }
    return 0;
}