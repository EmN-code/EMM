#include <stdio.h>
#include <msg.h>
#include <sched.h>
#include <thread.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

char sender_stack[THREAD_STACKSIZE];
char receiver_stack[THREAD_STACKSIZE];

msg_t msg_queue_receiver[8];

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue_receiver, 8);

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %ld\n", msg.content.value);
    }

    return NULL;
}

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;

    for (uint32_t i = 0; i < 10; ++i) {
        printf("Sending message with value: %ld\n", i);
        msg.content.value = i;
        msg_send(&msg, thread_get(2)); // assuming receiver_thread's PID is 2
        thread_sleep(1);
    }

    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid, sender_pid;
    
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), 
                                 THREAD_PRIORITY_MAIN - 1, 0, 
                                 receiver_thread, NULL, "receiver");
    
    sender_pid = thread_create(sender_stack, sizeof(sender_stack), 
                               THREAD_PRIORITY_MAIN, 0, 
                               sender_thread, NULL, "sender");

    (void)sender_pid;
    return 0;
}