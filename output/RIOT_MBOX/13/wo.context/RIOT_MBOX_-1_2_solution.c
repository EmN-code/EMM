#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define SENDER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)

static char sender_thread_stack[SENDER_THREAD_STACKSIZE];
static char receiver_thread_stack[RECEIVER_THREAD_STACKSIZE];

static msg_t msg_queue[8];
static msg_t msg;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        puts("Sender: Sending message...");
        msg.type = 1;
        msg.content.value = 1234;
        msg_send(&msg, thread_get(RECEIVER_THREAD_PRIORITY));
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value: %ld\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    puts("Starting message box example...");

    msg_init_queue(msg_queue, 8);

    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                  SENDER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender");

    thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                  RECEIVER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  receiver_thread, NULL, "receiver");

    return 0;
}