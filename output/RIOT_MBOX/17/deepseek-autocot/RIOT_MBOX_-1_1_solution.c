
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
        msg.content.value = 1; // Example event value
        msg_send(&msg, mbox_pid);
    }
}

int main(void) {
    // Initialize the message box
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Create the sender thread
    thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender_thread, NULL, "sender");

    // Main thread acts as the receiver
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received event with value: %d\n", msg.content.value);
    }

    return 0;
}
