#include "msg.h"
#include "thread.h"
#include "xtimer.h"

#define HIGH_PRIORITY_THREAD_STACK_SIZE   (THREAD_STACKSIZE_MAIN)
#define LOW_PRIORITY_THREAD_STACK_SIZE    (THREAD_STACKSIZE_MAIN)
#define HIGH_PRIORITY                     (THREAD_PRIORITY_MAIN - 1)
#define LOW_PRIORITY                      (THREAD_PRIORITY_MAIN - 2)
#define MSG_QUEUE_SIZE                    (8)

static char high_priority_thread_stack[HIGH_PRIORITY_THREAD_STACK_SIZE];
static char low_priority_thread_stack[LOW_PRIORITY_THREAD_STACK_SIZE];
static msg_t high_priority_msg_queue[MSG_QUEUE_SIZE];
static msg_t low_priority_msg_queue[MSG_QUEUE_SIZE];

void *high_priority_thread(void *arg) {
    msg_t msg;
    msg.content.value = 1;
    while (1) {
        // Sending a message to the low priority thread
        msg_send(&msg, (kernel_pid_t)(uintptr_t)arg);
        xtimer_sleep(3);
    }
    return NULL;
}

void *low_priority_thread(void *arg) {
    msg_t msg;
    while (1) {
        // Receiving message
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(high_priority_msg_queue, MSG_QUEUE_SIZE);
    msg_init_queue(low_priority_msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t low_pid = thread_create(low_priority_thread_stack, sizeof(low_priority_thread_stack),
                                        LOW_PRIORITY, 0, low_priority_thread, NULL, "low_priority_thread");
    
    thread_create(high_priority_thread_stack, sizeof(high_priority_thread_stack),
                  HIGH_PRIORITY, 0, high_priority_thread, (void *)(uintptr_t)low_pid, "high_priority_thread");
    
    return 0;
}