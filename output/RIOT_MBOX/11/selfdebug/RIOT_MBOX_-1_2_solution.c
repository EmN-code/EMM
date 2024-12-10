#include <ztimer.h>
#include <msg.h>
#include <xtimer.h>
#include <stdint.h>
#include <stdio.h>

#define INTERVAL (1U * ZTIMER_MSEC_PER_SEC)
#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static ztimer_periodic_t periodic_timer;
static mbox_t mbox;

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        ztimer_periodic_wait(&periodic_timer);
        msg.content.value = 42;  // Sample value for demonstration
        mbox_send(&mbox, &msg);
        printf("Message sent: %ld\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    ztimer_periodic_init(ZTIMER_MSEC, &periodic_timer, INTERVAL);

    thread_handler(NULL);

    return 0;
}