#include "thread.h"
#include "msg.h"
#include "stdio.h"
#include "xtimer.h"

#define MAIN_QUEUE_SIZE (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

char stack_one[THREAD_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE_PRINTF];
char stack_two[THREAD_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE_PRINTF];

kernel_pid_t pid_one;
kernel_pid_t pid_two;

void *thread_one(void *arg) {
    (void)arg;
    msg_t msg;
    msg_t reply;
    uint32_t count = 0;
    while (1) {
        msg_send_receive(&msg, &reply, pid_two);
        printf("Thread one: Message count %lu\n", (unsigned long)count++);
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    msg_t msg;
    msg_t reply;
    uint32_t count = 0;
    while (1) {
        msg_send_receive(&msg, &reply, pid_one);
        printf("Thread two: Message count %lu\n", (unsigned long)count++);
        xtimer_sleep(3);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    pid_one = thread_create(stack_one, sizeof(stack_one), 
                            THREAD_PRIORITY_MAIN - 1, 0,
                            thread_one, NULL, "thread_one");
    
    pid_two = thread_create(stack_two, sizeof(stack_two), 
                            THREAD_PRIORITY_MAIN - 2, 0,
                            thread_two, NULL, "thread_two");
    
    return 0;
}