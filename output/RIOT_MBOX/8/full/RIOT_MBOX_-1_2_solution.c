#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "string.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mma8x5x_regs.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *message_sender(void *arg) {
    (void)arg;
    int count = 0;
    msg_t msg;
    evtimer_mbox_t mbox;

    evtimer_mbox_init(&mbox, MAIN_QUEUE_SIZE);
    evtimer_msg_t event;
    event.offset = 2 * US_PER_SEC;

    while (1) {
        msg.content.value = count;
        msg_send(&msg, &mbox);
        printf("Sent message with value: %d\n", count);
        count++;
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    thread_create(message_sender_thread_stack, sizeof(message_sender_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, message_sender, NULL, "message_sender");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %ld\n", msg.content.value);
    }

    return 0;
}