#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)
#define MESSAGE_QUEUE_SIZE (8)

static msg_t msg_queue[MESSAGE_QUEUE_SIZE];
static mbox_t my_mbox;

void sender_thread(void) {
    msg_t msg;
    while (1) {
        printf("Sending message...\n");
        mbox_send(&my_mbox, &msg, true);
        sleep(2);
    }
}

void receiver_thread(void) {
    msg_t msg;
    while (1) {
        if (_mbox_get(&my_mbox, &msg, true) == 1) {
            printf("Received a message!\n");
        }
        sleep(1);
    }
}

int main(void) {
    puts("RIOT mailbox example\n");
    mbox_init(&my_mbox, msg_queue, MESSAGE_QUEUE_SIZE);
    
    char sender_stack[THREAD_STACKSIZE_MAIN];
    char receiver_stack[THREAD_STACKSIZE_MAIN];
    
    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0,
                  sender_thread, NULL, "sender");

    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, 0,
                  receiver_thread, NULL, "receiver");

    return 0;
}
