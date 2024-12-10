#include <thread.h>
#include <msg.h>
#include <stdio.h>
#include <xtimer.h>

#define THREAD_PRIORITY_MAIN   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_HIGH   (THREAD_PRIORITY_MAIN - 2)
#define THREAD_PRIORITY_LOW    (THREAD_PRIORITY_MAIN + 1)
#define MESSAGE_QUEUE_SIZE     (8)

static kernel_pid_t high_priority_thread_pid;
static kernel_pid_t low_priority_thread_pid;

static char high_priority_thread_stack[THREAD_STACKSIZE_MAIN];
static char low_priority_thread_stack[THREAD_STACKSIZE_MAIN];

static msg_t msg_queue[MESSAGE_QUEUE_SIZE];

void *high_priority_thread(void *arg) {
    msg_t msg;
    (void)arg;

    while (1) {
        if (msg_receive(&msg) < 0) {
            printf("High priority thread: Error receiving message\n");
            continue;
        }

        printf("High priority thread received message with content: %ld\n", msg.content.value);
        xtimer_sleep(1);
    }

    return NULL;
}

void *low_priority_thread(void *arg) {
    msg_t msg;
    (void)arg;
    msg.content.value = 42; // Example message data

    while (1) {
        msg_send(&msg, high_priority_thread_pid);
        printf("Low priority thread sent message with content: %ld\n", msg.content.value);
        xtimer_sleep(2);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MESSAGE_QUEUE_SIZE);

    high_priority_thread_pid = thread_create(high_priority_thread_stack, sizeof(high_priority_thread_stack),
                                             THREAD_PRIORITY_HIGH, 0, high_priority_thread, NULL, "high_priority");

    low_priority_thread_pid = thread_create(low_priority_thread_stack, sizeof(low_priority_thread_stack),
                                            THREAD_PRIORITY_LOW, 0, low_priority_thread, NULL, "low_priority");

    while (1) {
        // Main thread can perform other operations or remain idle
        xtimer_sleep(10);
    }
    return 0;
}