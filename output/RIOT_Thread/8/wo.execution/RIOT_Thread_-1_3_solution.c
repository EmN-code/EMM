#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <msg.h>

#define MSG_QUEUE_SIZE 8

// Stack size should be appropriate for two threads
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + 256)

// Define stack for threads
char stack1[STACK_SIZE];
char stack2[STACK_SIZE];

// Message queue for thread 2
msg_t msg_queue[MSG_QUEUE_SIZE];

void *thread_handler_one(void *arg) {
    (void)arg;
    msg_t msg;
    int cnt = 0;

    while (1) {
        msg.content.value = cnt;
        if (msg_send(&msg, thread_getpid()) == 1) {
            printf("Thread 1 sent message with value: %lu\n", (unsigned long) msg.content.value);
        }
        cnt++;
        ztimer_sleep(ZTIMER_SEC, 2);
    }
    return NULL;
}

void *thread_handler_two(void *arg) {
    (void)arg;
    msg_t msg;

    // Initialize message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Thread 2 received message with value: %lu\n", (unsigned long) msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1, thread2;

    // Create threads
    thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST, thread_handler_one,
                            NULL, "thread1");
    if (thread1 == 0) {
        printf("Failed to create thread 1\n");
        return 1;
    }

    thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                            THREAD_CREATE_STACKTEST, thread_handler_two,
                            NULL, "thread2");
    if (thread2 == 0) {
        printf("Failed to create thread 2\n");
        return 1;
    }

    // Simple scheduler emulation (in reality RIOT's scheduler will handle this)
    while (1) {
        thread_yield();
    }

    return 0;
}