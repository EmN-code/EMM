#include <xtimer.h>
#include <stdio.h>
#include <sched.h>
#include <msg.h>
#include <random.h>
#include <thread.h>

#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MBOX_SIZE (8)
#define INTERVAL (1000000)

static char stack[THREAD_STACKSIZE_MAIN];

static msg_t mbox_queue[MBOX_SIZE];
static mbox_t mbox;
static msg_t msg;

void *sender_thread(void *arg) {
    (void)arg;
    msg.type = 1;
    while (1) {
        printf("Sending message with type %d\n", msg.type);
        mbox_put(&mbox, &msg);
        xtimer_sleep(1);
        msg.type++;
    }
    return NULL;
}

int main(void) {
    msg_init_queue(mbox_queue, MBOX_SIZE);
    mbox_init(&mbox, mbox_queue, MBOX_SIZE);

    /* Create sender thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY, 0,
                  sender_thread, NULL, "sender_thread");

    while (1) {
        /* Simulate reception of a message */
        msg_t m;
        mbox_get(&mbox, &m);
        printf("Received message with type %d\n", m.type);
    } 
    return 0;
}
