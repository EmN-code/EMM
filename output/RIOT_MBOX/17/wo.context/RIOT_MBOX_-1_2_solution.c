#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char sender_stack[STACKSIZE];

static void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    for (unsigned i = 0; i < 5; i++) {
        msg.content.value = i;
        printf("[Sender] Sending message %u\n", (unsigned)msg.content.value);
        msg_send(&msg, thread_getpid);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t sender_pid;
    msg_t message_queue[8];
    
    msg_init_queue(message_queue, 8);
    
    sender_pid = thread_create(sender_stack, STACKSIZE, SENDER_PRIO, 0, sender, NULL, "sender");
    
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("[Receiver] Received message %u\n", (unsigned)msg.content.value);
    }
    
    return 0;
}