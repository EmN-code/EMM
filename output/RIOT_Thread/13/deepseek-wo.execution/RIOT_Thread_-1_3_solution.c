
#include <stdio.h>
#include <inttypes.h>
#include "msg.h"
#include "thread.h"

#define STACKSIZE 1024
#define NUM_MESSAGES 8

char stack1[STACKSIZE];
char stack2[STACKSIZE];

msg_t msg_array[NUM_MESSAGES];

void *thread1_func(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Thread 1 received message with value: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    msg_t msg;
    msg.content.value = 42;
    msg.type = 1;
    msg_send(&msg, (kernel_pid_t)arg);
    printf("Thread 2 sent message with value: %" PRIu32 "\n", msg.content.value);
    return NULL;
}

int main(void)
{
    msg_init_queue(msg_array, NUM_MESSAGES);

    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread1_func, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread2_func, (void *)pid1, "thread2");

    (void)pid2; // To avoid unused variable warning

    return 0;
}
