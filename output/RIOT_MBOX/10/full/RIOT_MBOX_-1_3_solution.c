#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "mma8x5x_regs.h"

#define MSG_QUEUE_SIZE  (8)
static msg_t msg_queue[MSG_QUEUE_SIZE];
static char stack[THREAD_STACKSIZE_MAIN];

void *msg_sender(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg.type = 0xABCD;  // Fill in a type ID
        msg_send(&msg, thread_getpid());
        printf("Message sent\n");
        ztimer_sleep(ZTIMER_SEC, 2);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    kernel_pid_t sender_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST, msg_sender,
                                            NULL, "msg_sender");
    (void)sender_pid;

    return 0;
}