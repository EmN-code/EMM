#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define THREAD_PRIORITY_HIGH   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW    (THREAD_PRIORITY_MAIN + 1)
#define STACK_SIZE             (THREAD_STACKSIZE_DEFAULT)
#define THREAD_MESSAGE_QUEUE_SIZE  (8)

static char high_priority_thread_stack[STACK_SIZE];
static char low_priority_thread_stack[STACK_SIZE];

static msg_t main_msg_queue[THREAD_MESSAGE_QUEUE_SIZE];

void *high_priority_thread(void *arg) {
    msg_t msg;
    (void)arg;
    
    while (1) {
        msg_receive(&msg);
        printf("High Priority Thread: Received message with content = %d\n", msg.content.value);
    }
    return NULL;
}

void *low_priority_thread(void *arg) {
    msg_t msg;
    (void)arg;
    
    msg.content.value = 42;
    msg_send(&msg, thread_getpid_of(main_thread));
    printf("Low Priority Thread: Sent message with content = %d\n", msg.content.value);
    
    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, THREAD_MESSAGE_QUEUE_SIZE);
    
    kernel_pid_t high_pid = thread_create(high_priority_thread_stack, STACK_SIZE,
                                          THREAD_PRIORITY_HIGH, THREAD_CREATE_STACKTEST,
                                          high_priority_thread, NULL, "high_priority");
    
    kernel_pid_t low_pid = thread_create(low_priority_thread_stack, STACK_SIZE,
                                         THREAD_PRIORITY_LOW, THREAD_CREATE_STACKTEST,
                                         low_priority_thread, NULL, "low_priority");
    
    printf("Threads started: high_priority pid = %d, low_priority pid = %d\n", high_pid, low_pid);
    
    return 0;
}