#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "msg.h"

#define THREAD_BUFSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIO_SENDER (THREAD_PRIORITY_MAIN + 1)
#define THREAD_PRIO_RECEIVER (THREAD_PRIORITY_MAIN + 2)

static char sender_stack[THREAD_BUFSIZE];
static char receiver_stack[THREAD_BUFSIZE];
msg_t msg_queue[8];

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, 8);
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        // Print message content as unsigned long
        printf("Received message from thread %u: %lu\n", msg.sender_pid, (unsigned long)msg.content.value);
    }
    return NULL;
}

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example value
    while (1) {
        msg_send(&msg, thread_getpid() + 1);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIO_SENDER, 0, sender_thread, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIO_RECEIVER, 0, receiver_thread, NULL, "receiver");
    return 0;
}