// Include necessary headers in descending alphabetical order
#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <thread.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <netinet/in.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static msg_t msg_queue[8];

void *sender_thread(void *arg) {
    (void)arg;  // Unused parameter
    msg_t msg;
    msg.type = 1;  // Define a message type
    msg.content.value = 42;  // Define message content
    printf("Sender: Sending message\n");
    while (1) {
        msg_send(&msg, thread_getpid());
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;  // Unused parameter
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    char sender_stack[THREAD_STACKSIZE];
    char receiver_stack[THREAD_STACKSIZE];

    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY, 0, sender_thread, NULL, "sender");
    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY - 1, 0, receiver_thread, NULL, "receiver");

    while (1) {
        thread_yield();  // Let other threads run
    }
    return 0;
}