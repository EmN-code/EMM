#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <thread.h>
#include <time_units.h>
#include <stdlib.h>
#include <stddef.h>
#include <msg.h>
#include <stdbool.h>
#include <stdio.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>

// Structure for a message box event
typedef struct {
    evtimer_msg_event_t evt;
    kernel_pid_t thread_pid;
} timed_event_t;

#define QUEUE_SIZE (8)
#define STACK_SIZE (THREAD_STACKSIZE_MAIN)
#define MSG_TYPE_EVENT (0x4545)

// Global Variables
static msg_t queue[QUEUE_SIZE];
mbox_t mbox;
char recv_thread_stack[STACK_SIZE];

// Thread stack for message sending
char send_thread_stack[STACK_SIZE];

void* recv_thread(void* arg){
    (void) arg;
    msg_t msg;
    while (1) {
        // Wait and retrieve the message from the mailbox
        if (_mbox_get(&mbox, &msg, 1)) {
            printf("Received message with type: %u\n", msg.type);
        }
        xtimer_sleep(1);
    }
    return NULL;
}

void* send_thread(void* arg){
    (void) arg;
    static unsigned tick = 0;
    msg_t msg;
    msg.type = MSG_TYPE_EVENT;
    msg.content.value = tick;

    while (1) {
        msg.content.value = tick;
        tick++;
        msg_send_receive(&msg, &msg, thread_getpid());
        printf("Sent message with content: %lu\n", msg.content.value);
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    // Initialize mailbox
    msg_init_queue(queue, QUEUE_SIZE);

    // Start receiving thread
    thread_create(recv_thread_stack, sizeof(recv_thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, recv_thread, NULL, "recv");

    // Start sending thread
    thread_create(send_thread_stack, sizeof(send_thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, send_thread, NULL, "send");

    return 0;
}