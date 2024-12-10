#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stddef.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <thread.h>

#define SENDER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN)

msg_t msg_queue[8];
char sender_stack[SENDER_THREAD_STACKSIZE];
char receiver_stack[RECEIVER_THREAD_STACKSIZE];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    for (int i = 0; i < 10; i++) {
        msg.content.value = i;
        printf("Sender: Sending message with value %d\n", i);
        msg_send(&msg, thread_getpid_of((thread_t *)arg));
        ztimer_sleep(ZTIMER_USEC, 1000000U);  // Sleep for 1 second
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %ld\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    kernel_pid_t receiver_pid = thread_create(
        receiver_stack, RECEIVER_THREAD_STACKSIZE, RECEIVER_THREAD_PRIORITY,
        THREAD_CREATE_STACKTEST, receiver, NULL, "receiver_thread");

    thread_create(
        sender_stack, SENDER_THREAD_STACKSIZE, SENDER_THREAD_PRIORITY,
        THREAD_CREATE_STACKTEST, sender, &receiver_pid, "sender_thread");

    return 0;
}
