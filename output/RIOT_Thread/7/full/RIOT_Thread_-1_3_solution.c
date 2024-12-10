#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "time_units.h"
#include "mma8x5x_regs.h"

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 2)
#define MSG_QUEUE_SIZE 8

char stack[THREAD_STACKSIZE_MAIN];
msg_t msg_queue[MSG_QUEUE_SIZE];

void send_thread(void) {
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        printf("Sending message with value: %d\n", (int)msg.content.value);
        msg_send(&msg, thread_getpid());
        xtimer_sleep(1);
    }
}

void receive_thread(void) {
    msg_t msg;

    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received message with value: %d\n", (int)msg.content.value);
        }
    }
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_HIGH, 0, send_thread, NULL, "send_thread");
    receive_thread();

    return 0;
}