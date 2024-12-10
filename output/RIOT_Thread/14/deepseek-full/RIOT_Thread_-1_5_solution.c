
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define STACKSIZE 512
#define MSG_QUEUE_SIZE 16

typedef struct {
    uint32_t value;
} msg_t;

typedef void (*thread_task_func_t)(void *);

typedef struct {
    thread_task_func_t task_func;
    void *arg;
} thread_t;

typedef uint16_t kernel_pid_t;

static kernel_pid_t high_priority_pid;
static kernel_pid_t low_priority_pid;

static msg_t msg_queue[MSG_QUEUE_SIZE];

void high_priority_thread(void *arg) {
    msg_t msg;
    while (1) {
        // Simulate receiving a message
        printf("High priority thread received message: %lu\n", msg.value);
    }
}

void low_priority_thread(void *arg) {
    msg_t msg;
    while (1) {
        // Simulate receiving a message
        printf("Low priority thread received message: %lu\n", msg.value);
    }
}

kernel_pid_t thread_create(char *stack, int stacksize, uint8_t priority, int flags, thread_task_func_t task_func, void *arg, const char *name) {
    thread_t *thread = (thread_t *)malloc(sizeof(thread_t));
    thread->task_func = task_func;
    thread->arg = arg;
    return (kernel_pid_t)thread;
}

void msg_init_queue(msg_t *array, int num) {
    // Initialize the message queue
}

int main(void) {
    char high_stack[STACKSIZE];
    char low_stack[STACKSIZE];

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    high_priority_pid = thread_create(high_stack, STACKSIZE, 1, 0, high_priority_thread, NULL, "high_priority_thread");
    low_priority_pid = thread_create(low_stack, STACKSIZE, 2, 0, low_priority_thread, NULL, "low_priority_thread");

    msg_t msg;
    for (int i = 0; i < 10; i++) {
        msg.value = i;
        if (i % 2 == 0) {
            // Simulate sending a message to the high priority thread
            printf("Sending message %lu to high priority thread\n", msg.value);
        } else {
            // Simulate sending a message to the low priority thread
            printf("Sending message %lu to low priority thread\n", msg.value);
        }
        usleep(100000); // Simulate delay
    }

    return 0;
}
