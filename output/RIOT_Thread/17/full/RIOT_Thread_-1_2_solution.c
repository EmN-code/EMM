#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define SENDER_PRIO            (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO          (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE_MAIN         THREAD_STACKSIZE_MAIN

char stack_sender[STACKSIZE_MAIN];
char stack_receiver[STACKSIZE_MAIN];

static msg_t queue[8];

static void *sender(void *arg) {
    msg_t msg;
    int count = 0;

    while (1) {
        msg.content.value = count++;
        if (msg_send(&msg, thread_get(RECEIVER_PRIO)) < 1) {
            printf("Message send failed\n");
        } else {
            printf("Sent: %d\n", msg.content.value);
        }
        xtimer_sleep(1);
    }
    return NULL;
}

static void *receiver(void *arg) {
    msg_t msg;
    msg_init_queue(queue, sizeof(queue) / sizeof(msg_t));

    while (1) {
        msg_receive(&msg);
        printf("Received: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    puts("RIOT messaging example\n");

    thread_create(stack_sender, sizeof(stack_sender),
                  SENDER_PRIO, 0, sender, NULL, "sender");

    thread_create(stack_receiver, sizeof(stack_receiver),
                  RECEIVER_PRIO, 0, receiver, NULL, "receiver");

    return 0;
}