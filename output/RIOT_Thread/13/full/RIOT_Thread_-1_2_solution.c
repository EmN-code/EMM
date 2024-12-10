#include "msg.h"
#include "mma8x5x_regs.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "thread.h"

#include <stdlib.h>     // Required for malloc
#include <string.h>     // Required for memset
#include <xtimer.h>     // Required for xtimer_sleep

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)

char stack_thread1[THREAD_STACKSIZE_MAIN];
char stack_thread2[THREAD_STACKSIZE_MAIN];

msg_t msg_q1[8];
msg_t msg_q2[8];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

/* Message structure */
typedef struct {
    int message_id;
} message_t;

/* Thread function for sending messages */
void *thread1_handler(void *arg) {
    (void)arg;
    msg_t msg;
    message_t my_message;
    my_message.message_id = 1;

    printf("Thread 1 running\n");

    while (1) {
        msg.content.value = my_message.message_id;
        msg_send(&msg, thread2_pid);
        my_message.message_id++;
        printf("Thread 1 sent message with ID: %d\n", my_message.message_id);
        xtimer_sleep(1);  // Delay of 1 second
    }
    return NULL;
}

/* Thread function for receiving messages */
void *thread2_handler(void *arg) {
    (void)arg;
    msg_t msg;

    printf("Thread 2 running\n");

    while (1) {
        msg_receive(&msg);
        printf("Thread 2 received message with ID: %ld\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    /* Initialize message queues */
    msg_init_queue(msg_q1, 8);
    msg_init_queue(msg_q2, 8);

    /* Create thread 1 for sending messages */
    thread1_pid = thread_create(stack_thread1, sizeof(stack_thread1),
                                THREAD_PRIORITY_SENDER, 0,
                                thread1_handler, NULL, "thread1");

    /* Create thread 2 for receiving messages */
    thread2_pid = thread_create(stack_thread2, sizeof(stack_thread2),
                                THREAD_PRIORITY_RECEIVER, 0,
                                thread2_handler, NULL, "thread2");

    /* Main thread loop */
    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}