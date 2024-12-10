#include <xtimer.h>
#include <stdio.h>
#include <msg.h>

#define INTERVAL (1U * US_PER_SEC) // 1 second interval
#define MAIN_QUEUE_SIZE (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void sender_thread(void) {
    while (1) {
        msg_t msg;
        msg_send(&msg, (kernel_pid_t)thread_getpid());
        printf("Message sent\n");
        xtimer_sleep(INTERVAL);
    }
}

void receiver_thread(void) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Message received\n");
    }
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    
    kernel_pid_t sender = thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                                        THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                        sender_thread, NULL, "sender");

    kernel_pid_t receiver = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                                          THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                          receiver_thread, NULL, "receiver");

    (void)sender;
    (void)receiver;

    return 0;
}