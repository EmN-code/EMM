#include <stdio.h>
#include <thread.h>
#include <msg.h>

#define THREAD_STACKSIZE_DEFAULT  (1024)
#define THREAD_PRIORITY_SENDER    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER  (THREAD_PRIORITY_MAIN - 2)
#define MAIN_QUEUE_SIZE           (8)

static char sender_thread_stack[THREAD_STACKSIZE_DEFAULT];
static char receiver_thread_stack[THREAD_STACKSIZE_DEFAULT];

msg_t _msg_queue[MAIN_QUEUE_SIZE];

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    for (int i = 0; i < 5; i++) {
        msg.content.value = i;
        printf("[Sender] Sending message %d\n", i);
        msg_send(&msg, thread_getpid_of(THREAD_PRIORITY_RECEIVER));
    }
    return NULL;
}

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("[Receiver] Received message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_msg_queue, MAIN_QUEUE_SIZE);

    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                  THREAD_PRIORITY_SENDER, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender_thread");

    thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                  THREAD_PRIORITY_RECEIVER, THREAD_CREATE_STACKTEST,
                  receiver_thread, NULL, "receiver_thread");

    return 0;
}