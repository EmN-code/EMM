
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <esp_system.h>
#include <thread.h>
#include <msg.h>

#define STACKSIZE 1024
#define NUM_MESSAGES 8

msg_t msg_queue[NUM_MESSAGES];

void thread_func_1(void *arg)
{
    (void)arg;
    printf("Thread 1 started\n");
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Thread 1 received message\n");
    }
}

void thread_func_2(void *arg)
{
    (void)arg;
    printf("Thread 2 started\n");
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Thread 2 received message\n");
    }
}

int main(void)
{
    msg_init_queue(msg_queue, NUM_MESSAGES);

    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    kernel_pid_t pid1 = thread_create(stack1, STACKSIZE, PRIORITY_MAIN - 1, 0, thread_func_1, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, STACKSIZE, PRIORITY_MAIN - 2, 0, thread_func_2, NULL, "thread2");

    printf("Threads created: %d, %d\n", pid1, pid2);

    // Change priority of thread 2 after some time
    sleep(5);
    thread_set_priority(pid2, PRIORITY_MAIN - 1);
    printf("Changed priority of thread 2 to higher\n");

    // Send messages to threads
    msg_t msg;
    msg.content.value = 0;
    msg_send(&msg, pid1);
    msg_send(&msg, pid2);

    return 0;
}
