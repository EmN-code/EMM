#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"

#define SENDER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char sender_thread_stack[SENDER_THREAD_STACKSIZE];
static char receiver_thread_stack[RECEIVER_THREAD_STACKSIZE];

static msg_t msg_queue[8];
static kernel_pid_t sender_pid;
static kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Simple example message content
    msg.type = 1; // Set a message type

    while (1) {
        msg_send(&msg, receiver_pid); // Sending the message to receiver
        printf("Sender: Sent message with value %lu\n", msg.content.value);
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1000 ms
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    while (1) {
        msg_receive(&msg); // Receive message
        printf("Receiver: Received message with value %lu\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack), 
                               THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, NULL, "receiver");

    sender_pid = thread_create(sender_thread_stack, sizeof(sender_thread_stack), 
                               THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000); // Main loop sleeps to keep other threads running
    }

    return 0;
}