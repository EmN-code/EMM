#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE (THREAD_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE_PRINTF)
#define QUEUE_SIZE (8)

static char sender_stack[STACKSIZE];
static char receiver_stack[STACKSIZE];

static msg_t receiver_queue[QUEUE_SIZE];

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(receiver_queue, QUEUE_SIZE);
    
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received msg.type: %lu\n", (unsigned long)msg.type);
        }
    }
    
    return NULL;
}

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;
    
    while (1) {
        msg_send(&msg, thread_get(RECEIVER_THREAD_PID));
        printf("Sent msg.type: %lu\n", (unsigned long)msg.type);
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO,
                  THREAD_CREATE_STACKTEST, receiver, NULL, "receiver");
    
    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO,
                  THREAD_CREATE_STACKTEST, sender, NULL, "sender");

    while (1) {}
    return 0;
}
