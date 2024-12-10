#include "esp_system.h"
#include "event/thread.h"
#include "msg.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)

char stack_sender[THREAD_STACKSIZE_MAIN];
char stack_receiver[THREAD_STACKSIZE_MAIN];

msg_t msg;
msg_t queue[8];

void *thread_sender(void *arg) {
    (void)arg;
    
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    printf("Sending message...\n");
    msg_send_receive(&msg, &msg, thread_getpid_of_name("receiver"));

    return NULL;
}

void *thread_receiver(void *arg) {
    (void)arg;

    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message of type %d with value %" PRIu32 "\n", msg.type, msg.content.value);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, sizeof(queue) / sizeof(queue[0]));

    thread_create(stack_sender, sizeof(stack_sender), THREAD_PRIORITY_SENDER, 0,
                  thread_sender, NULL, "sender");
    thread_create(stack_receiver, sizeof(stack_receiver), THREAD_PRIORITY_RECEIVER, 0,
                  thread_receiver, NULL, "receiver");

    return 0;
}