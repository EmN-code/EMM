#include <thread.h>
#include <msg.h>
#include <kernel.h>
#include <stdio.h>

#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW (THREAD_PRIORITY_MAIN - 2)
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define MSG_QUEUE_SIZE (8)

char stack_sender[STACK_SIZE];
char stack_receiver[STACK_SIZE];

msg_t msg_queue[MSG_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t message;
    message.type = 0x1234;
    while (1) {
        printf("Sending message...\n");
        msg_send(&message, thread_getpid());
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t message;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    while (1) {
        msg_receive(&message);
        printf("Received message with type: %x\n", message.type);
    }
    return NULL;
}

int main(void) {
    puts("RIOT OS messaging example\n");
    
    thread_create(stack_sender, sizeof(stack_sender), THREAD_PRIORITY_HIGH, 0,
                  sender_thread, NULL, "sender");
    
    thread_create(stack_receiver, sizeof(stack_receiver), THREAD_PRIORITY_LOW, 0,
                  receiver_thread, NULL, "receiver");

    return 0;
}