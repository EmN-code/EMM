#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <msg.h>

#define HIGH_PRI_THREAD_PRIORITY    (THREAD_PRIORITY_MAIN - 1)
#define LOW_PRI_THREAD_PRIORITY     (THREAD_PRIORITY_MAIN + 1)
#define THREAD_STACKSIZE_DEFAULT    (1024)
#define MSG_QUEUE_SIZE              (8)

static char high_priority_thread_stack[THREAD_STACKSIZE_DEFAULT];
static char low_priority_thread_stack[THREAD_STACKSIZE_DEFAULT];

static msg_t msg_queue[MSG_QUEUE_SIZE];

// High Priority Thread Function
void *high_pri_thread(void *arg) {
    (void)arg;
    msg_t msg, reply;
    printf("High priority thread started.\n");

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
        // Send a reply back
        reply.type = 0;
        reply.content.value = msg.content.value + 1;
        msg_reply(&msg, &reply);
    }
    return NULL;
}

// Low Priority Thread Function
void *low_pri_thread(void *arg) {
    (void)arg;
    msg_t msg, reply;
    kernel_pid_t high_pri_thread_pid = *((kernel_pid_t *)arg);
    printf("Low priority thread started.\n");

    while (1) {
        // Send a message to the high priority thread
        msg.type = 0;
        msg.content.value = rand() % 100;
        printf("Low priority thread sending message: %d\n", msg.content.value);
        msg_send_receive(&msg, &reply, high_pri_thread_pid);
        printf("Low priority thread received reply: %d\n", reply.content.value);
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t high_pri_thread_pid;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create high priority thread
    high_pri_thread_pid = thread_create(high_priority_thread_stack, sizeof(high_priority_thread_stack),
                                        HIGH_PRI_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                        high_pri_thread, NULL, "high_pri_thread");
    
    // Create low priority thread
    thread_create(low_priority_thread_stack, sizeof(low_priority_thread_stack),
                  LOW_PRI_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  low_pri_thread, &high_pri_thread_pid, "low_pri_thread");

    return 0;
}