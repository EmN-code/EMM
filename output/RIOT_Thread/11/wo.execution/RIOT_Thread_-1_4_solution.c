#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "thread.h"
#include "timex.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "string.h"
#include "event/thread.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "kernel_defines.h"
#include "mma8x5x_regs.h"
#include "time_units.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

#define SENDER_PRIO            (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO          (THREAD_PRIORITY_MAIN - 2)

msg_t msg_queue[8];
char sender_stack[THREAD_STACKSIZE_MAIN];
char receiver_stack[THREAD_STACKSIZE_MAIN];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    printf("Sender: sending message with value %d\n", (int)msg.content.value);
    msg_send(&msg, thread_getpid());
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    msg_receive(&msg);
    printf("Receiver: received message with value %ld\n", (long int)msg.content.value);
    return NULL;
}

int main(void) {
    puts("Inter-thread communication example");

    msg_init_queue(msg_queue, 8);

    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0, sender, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, 0, receiver, NULL, "receiver");

    return 0;
}