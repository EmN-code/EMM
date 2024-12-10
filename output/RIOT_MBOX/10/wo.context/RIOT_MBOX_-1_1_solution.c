#include <ztimer.h>
#include <stdio.h>
#include <msg.h>
#include <xtimer.h>
#include <thread.h>

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 2)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 3)

#define INTERVAL_USEC (1000000U) // 1 second

static msg_t msg_queue[8];
static char sender_stack[THREAD_STACKSIZE_DEFAULT];

static void *_sender_thread(void *arg) {
    (void) arg; // Not used
    msg_t msg;

    while (1) {
        xtimer_usleep(INTERVAL_USEC); // Wait for the specified interval
        msg_send(&msg, thread_getpid()); // Send message to the main thread
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0, _sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        msg_receive(&msg); // Blocking receive
        printf("Received a message!\n");
    }

    return 0;
}