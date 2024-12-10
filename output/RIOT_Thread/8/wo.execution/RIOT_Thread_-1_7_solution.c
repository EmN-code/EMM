#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <thread.h>
#include <timex.h>
#include <time_units.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <msg.h>

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define SENDER_PRIO             (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO           (THREAD_PRIORITY_MAIN - 2)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

static msg_t sender_msg_queue[8];
static msg_t receiver_msg_queue[8];

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    for (size_t i = 0; i < 10; i++) {
        msg.content.value = i;
        printf("Sending message: %lu\n", (unsigned long)i);
        msg_send(&msg, thread_getpid());
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    return NULL;
}

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message: %lu\n", (unsigned long)msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(sender_msg_queue, 8);
    msg_init_queue(receiver_msg_queue, 8);

    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, 0,
                  receiver_thread, NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0,
                  sender_thread, NULL, "sender");

    return 0;
}