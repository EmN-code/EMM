#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)

char stack[THREAD_STACKSIZE_MAIN];
msg_t msg_queue[8];
msg_t msg;

void sender_thread(void) {
    msg_t msg;
    msg.type = 1;  // Arbitrary type

    while (1) {
        msg.content.value = 123;  // Some arbitrary value
        msg_send(&msg, thread_getpid().pid + 1);
        xtimer_sleep(2);
    }
}

void receiver_thread(void) {
    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", msg.content.value);
    }
}

int main(void) {
    kernel_pid_t sender_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");
    kernel_pid_t receiver_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    (void)sender_pid;
    (void)receiver_pid;

    while (1) {
        // Main thread can perform other tasks or stay idle.
        xtimer_sleep(5);
    }

    return 0;
}