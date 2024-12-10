#include <arpa/inet.h> 
#include <dht.h>
#include <dht_params.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <msg.h>
#include <MQTTClient.h>
#include <netinet/in.h>
#include <paho_mqtt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <thread.h>
#include <timex.h>
#include <time_units.h>
#include <unistd.h>
#include <xtimer.h>
#include <ztimer.h>

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SENDER  (THREAD_PRIORITY_MAIN - 2)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 3)

#define MESSAGE_QUEUE_SIZE (8)

void *sender_thread(void *arg);
void *receiver_thread(void *arg);

msg_t msg_queue[MESSAGE_QUEUE_SIZE];
msg_t msg;
msg_t reply;

int main(void) {
    msg_init_queue(msg_queue, MESSAGE_QUEUE_SIZE);

    char receiver_stack[THREAD_STACKSIZE_MAIN];
    char sender_stack[THREAD_STACKSIZE_MAIN];

    kernel_pid_t receiver_pid = thread_create(
        receiver_stack, sizeof(receiver_stack),
        THREAD_PRIORITY_RECEIVER, 0,
        receiver_thread, NULL, "receiver"
    );

    kernel_pid_t sender_pid = thread_create(
        sender_stack, sizeof(sender_stack),
        THREAD_PRIORITY_SENDER, 0,
        sender_thread, (void *)receiver_pid, "sender"
    );

    (void) sender_pid; // to avoid unused variable warning

    while(1) {
        thread_yield();
    }

    return 0;
}

void *sender_thread(void *arg) {
    kernel_pid_t receiver_pid = (kernel_pid_t) arg;
    int count = 0;

    while (1) {
        msg.content.value = count++;
        if (msg_send_receive(&msg, &reply, receiver_pid) == 1) {
            printf("Sender: Sent message with value: %d and received reply with value: %d\n", msg.content.value, reply.content.value);
        }
        xtimer_sleep(1);
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    (void) arg;

    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Receiver: Received message with value: %d\n", msg.content.value);
            reply.content.value = msg.content.value + 1;  // Example of processing
            msg_reply(&msg, &reply);
            printf("Receiver: Replied with value: %d\n", reply.content.value);
        }
    }

    return NULL;
}