#include "thread.h"
#include "msg.h"
#include <stdio.h>

#define MAIN_QUEUE_SIZE     (64)
#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO       (THREAD_PRIORITY_MAIN - 2)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

static void *sender(void *arg);
static void *receiver(void *arg);

int main(void) 
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    
    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                                             SENDER_PRIO, THREAD_CREATE_STACKTEST,
                                             sender, NULL, "sender");
    
    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating sender thread\n");
        return 1;
    }

    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                              RECEIVER_PRIO, THREAD_CREATE_STACKTEST,
                                              receiver, NULL, "receiver");
    
    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating receiver thread\n");
        return 1;
    }

    return 0;
}

static void *sender(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = 42;  // Arbitrary message content
        if (msg_send(&msg, thread_get_pid()) < 1) {
            printf("Failed to send message\n");
        } else {
            printf("Message sent\n");
        }
        xtimer_sleep(1);
    }
    return NULL;
}

static void *receiver(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message: %ld\n", msg.content.value);
    }
    return NULL;
}