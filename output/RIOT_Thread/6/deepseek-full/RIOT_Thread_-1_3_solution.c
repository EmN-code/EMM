
#include "xtimer.h"
#include "evtimer_mbox.h"
#include "thread.h"
#include "timex.h"
#include "ztimer.h"
#include "mma8x5x_regs.h"
#include "msg.h"
#include "mbox.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define STACKSIZE 512
#define NUM_MESSAGES 16

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];
static msg_t msg_array[NUM_MESSAGES];
static msg_t msg_queue[NUM_MESSAGES];

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 0x12345678;

    while (1) {
        msg_send_receive(&msg, &msg, thread_getpid());
        xtimer_sleep(1);
    }
}

void receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with type %u and value %u\n", msg.type, msg.content.value);
    }
}

int main(void) {
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t sender_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    kernel_pid_t receiver_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                              THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    (void)sender_pid;
    (void)receiver_pid;

    return 0;
}
