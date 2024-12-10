#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <mbox.h>

#define QUEUE_SIZE 8
#define THREAD_STACKSIZE 1024

static msg_t msg_queue[QUEUE_SIZE];
static char thread_stack[THREAD_STACKSIZE];

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    mbox_t mbox;

    mbox_init(&mbox, msg_queue, QUEUE_SIZE);
    printf("Thread: Message box initialized.\n");

    while (1) {
        /* Block until a message is received */
        msg_receive(&msg);
        printf("Thread: Received message with data: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid;
    msg_t msg;

    /* Initialize message queue */
    msg_init_queue(msg_queue, QUEUE_SIZE);

    /* Create a thread to handle messages */
    thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST,
                               thread_handler, NULL, "msg_thread");
    if (thread_pid < 0) {
        printf("Error: Failed to create thread\n");
        return 1;
    }

    printf("Main: Thread created, sending messages to message box.\n");

    for (int i = 0; i < 5; i++) {
        /* Prepare the message */
        msg.content.value = i;
        /* Send the message to the message queue */
        msg_send(&msg, thread_pid);
        printf("Main: Sent message with data: %d\n", i);
        /* Wait for 1 second */
        xtimer_sleep(1);
    }

    return 0;
}