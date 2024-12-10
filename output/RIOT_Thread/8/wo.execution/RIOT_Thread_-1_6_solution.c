#include <ztimer.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN    (CONFIG_MAIN_THREAD_STACKSIZE)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_CREATE_STACKTEST  (1 << 15)

msg_t queue[8];

void sender_thread(void *arg){
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // An example payload
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;
    msg_send(&msg, receiver_pid);
}

void receiver_thread(void *arg){
    (void)arg;
    msg_t msg;
    msg_init_queue(queue, 8);
    while(1){
        msg_receive(&msg);
        printf("Received: %ld\n", msg.content.value);
    }
}

int main(void){
    char sender_stack[THREAD_STACKSIZE_MAIN];
    char receiver_stack[THREAD_STACKSIZE_MAIN];
    kernel_pid_t receiver_pid = thread_create(receiver_stack, THREAD_STACKSIZE_MAIN,
                                              THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                                              receiver_thread, NULL, "receiver");
    thread_create(sender_stack, THREAD_STACKSIZE_MAIN,
                  THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                  sender_thread, &receiver_pid, "sender");
    return 0;
}