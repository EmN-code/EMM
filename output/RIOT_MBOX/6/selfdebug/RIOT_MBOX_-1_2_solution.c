#include <ztimer.h>
#include <msg.h>
#include <stdio.h>

#define INTERVAL_US 1000000
#define QUEUE_SIZE 8

static msg_t queue[QUEUE_SIZE];
static mbox_t mbox;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0;

    ztimer_periodic_t timer;
    ztimer_periodic_init(ZTIMER_USEC, &timer, INTERVAL_US);
    
    while (1) {
        ztimer_periodic_wait(&timer);
        msg.content.value += 1;
        mbox_send(&mbox, &msg);
        printf("Message sent: %lu\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    printf("msg box example
");

    mbox_init(&mbox, queue, QUEUE_SIZE);

    char sender_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL,
                  "sender_thread");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message: %lu\n", msg.content.value);
    }

    return 0;
}