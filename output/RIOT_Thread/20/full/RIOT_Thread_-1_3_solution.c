#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define THREAD_STACKSIZE_MAIN THREAD_STACKSIZE_DEFAULT
#define MY_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

static void *thread1_handler(void *arg) {
    (void)arg;
    msg_t msg;

    // Simulating message sending in a loop
    while (1) {
        msg.content.value = 1;
        msg_send(&msg, thread_getpid_of((thread_t *)arg));
        xtimer_sleep(1);
    }

    return NULL;
}

static void *thread2_handler(void *arg) {
    (void)arg;
    msg_t msg;

    // Continuously receive messages and process
    while (1) {
        msg_receive(&msg);
        printf("Received message with content: %ld\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, 
                                thread2_handler, NULL, "thread2");
    thread1_pid = thread_create(stack1, sizeof(stack1), MY_THREAD_PRIORITY, THREAD_CREATE_STACKTEST, 
                                thread1_handler, (void *)thread2_pid, "thread1");

    while (1) {
        // The main thread doesn't need to do anything in this case
        xtimer_sleep(10);
    }

    return 0;
}