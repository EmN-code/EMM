#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"

#define STACKSIZE (THREAD_STACKSIZE_MAIN)

char thread_stack1[STACKSIZE];
char thread_stack2[STACKSIZE];

kernel_pid_t thread_pid1;
kernel_pid_t thread_pid2;

void *thread1(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    printf("Thread 1: Sending message to thread 2.\n");
    while (msg_try_send(&msg, thread_pid2) != 1) {
        printf("Thread 1: Retrying message send to thread 2.\n");
        xtimer_sleep(1);
    }
    printf("Thread 1: Message sent successfully to thread 2.\n");

    return NULL;
}

void *thread2(void *arg) {
    (void)arg;
    msg_t msg;

    printf("Thread 2: Waiting to receive message.\n");
    msg_receive(&msg);
    printf("Thread 2: Received message with value %lu\n", msg.content.value);

    return NULL;
}

int main(void) {
    // Create threads with different priorities
    thread_pid1 = thread_create(thread_stack1, sizeof(thread_stack1), THREAD_PRIORITY_MAIN - 1, 0, thread1, NULL, "thread1");
    thread_pid2 = thread_create(thread_stack2, sizeof(thread_stack2), THREAD_PRIORITY_MAIN - 2, 0, thread2, NULL, "thread2");

    while (1) {
        // Let the threads run
        xtimer_sleep(5);
    }
    
    return 0;
}