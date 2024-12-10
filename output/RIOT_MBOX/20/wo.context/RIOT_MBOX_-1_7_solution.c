#include <thread.h>
#include <stdio.h>
#include <msg.h>
#include <kernel.h>
#include <event.h>

#define MAIN_QUEUE_SIZE     (8)
#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO       (THREAD_PRIORITY_MAIN)
#define INTERVAL            (1U) /* 1 second */

static char sender_stack[THREAD_STACKSIZE_DEFAULT];
static char receiver_stack[THREAD_STACKSIZE_DEFAULT];

/* message queue for the main thread */
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

/* mailbox structure */
static mbox_t _mbox;
static msg_t _mbox_queue[MAIN_QUEUE_SIZE];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = 42;  /* sample message content */
        mbox_send(&_mbox, &msg);
        printf("Message sent with value: %d\n", msg.content.value);
        xtimer_sleep(INTERVAL);
    }

    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_receive(&_mbox, &msg);
        printf("Message received with value: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    mbox_init(&_mbox, _mbox_queue, MAIN_QUEUE_SIZE);
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    printf("RIOT message box example\n");

    /* start sender and receiver threads */
    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO,
                  THREAD_CREATE_STACKTEST, sender, NULL, "sender");
    
    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO,
                  THREAD_CREATE_STACKTEST, receiver, NULL, "receiver");

    /* Start the main scheduler */
    msg_t msg;
    msg_receive(&msg);

    return 0;
}