#include <stdio.h>
#include <msg.h>
#include <thread.h>
#include <xtimer.h>

#define SENDER_STACKSIZE    (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define RECEIVER_STACKSIZE  (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define SENDER_PRIORITY     (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIORITY   (THREAD_PRIORITY_MAIN - 2)

static char sender_stack[SENDER_STACKSIZE];
static char receiver_stack[RECEIVER_STACKSIZE];

static msg_t msg_queue[8];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    while (1) {
        printf("Sender: Sending message...\n");
        if (msg_try_send(&msg, thread_get(receiver_pid)) <= 0){
            printf("Sender: Failed to send\n");
        }
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, 8);
    while (1) {
        if (msg_receive(&msg) < 0) {
            printf("Receiver: Failed to receive\n");
            continue;
        }
        printf("Receiver: Received message with value %ld\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_stack, RECEIVER_STACKSIZE, 
                                              RECEIVER_PRIORITY, THREAD_CREATE_STACKTEST,
                                              receiver_thread, NULL, "receiver");

    thread_create(sender_stack, SENDER_STACKSIZE, 
                  SENDER_PRIORITY, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender");

    return 0;
}