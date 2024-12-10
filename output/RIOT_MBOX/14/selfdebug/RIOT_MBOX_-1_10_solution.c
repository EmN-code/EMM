#include <thread.h>  // For threading related functions.
#include <stdio.h>    // For standard input-output functions.
#include <evtimer.h>   // For event timer management.
#include <msg.h>      // For message handling.

#define MSG_QUEUE_SIZE 8

msg_t msg_queue[MSG_QUEUE_SIZE];
static kernel_pid_t sender_pid;
static evtimer_t evtimer;

/* Thread stack and priority definition */
#define THREAD_STACKSIZE    (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY     (THREAD_PRIORITY_MAIN - 1)

static char sender_thread_stack[THREAD_STACKSIZE];

void sender_thread(void *arg) {
    (void)arg;
    msg_t m;
    while (1) {
        printf("Sending message from sender_thread\n");
        evtimer_add(&evtimer, 5 * 1000 * US_PER_MS);  // Adding to event timer every 5 sec
        msg_receive(&m);  // Blocking receive for messages.
        printf("Received message in sender_thread\n");
    }
}

int main(void) {
    printf("Starting RIOT Message Box example\n");
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    event_thread_init_msg(&evtimer);  // Initialize the event timer handling

    sender_pid = thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                               THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                               sender_thread, NULL, "sender_thread");

    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Error initializing sender thread!\n");
        return 1;
    }

    /* Wait for the sender_thread to process */
    msg_t m;
    while (1) {
        printf("Main thread waiting for a message\n");
        msg_receive(&m);
        printf("Main thread received a message\n");
    }

    return 0;
}