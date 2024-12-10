# Import necessary modules and headers for ESP32 development with RIOT
#include <stdlib.h>
#include <stdio.h>
#include <msg.h>
#include <ztimer.h>
#include <xtimer.h>
#include <string.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <kernel_defines.h>

void *sender_thread(void *arg) {
    (void)arg; // unused parameter
    msg_t message;
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;

    while (1) {
        message.content.value = ztimer_now(ZTIMER_MSEC);
        msg_send(&message, receiver_pid);
        printf("Message sent with timestamp: %ld\n", message.content.value);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg; // unused parameter
    msg_t message_queue[8];
    msg_t message;

    msg_init_queue(message_queue, 8);

    while (1) {
        msg_receive(&message);
        printf("Received message with timestamp: %ld\n", message.content.value);
    }
    return NULL;
}

int main(void) {
    char sender_stack[THREAD_STACKSIZE_MAIN];
    char receiver_stack[THREAD_STACKSIZE_MAIN];

    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), 
                                              THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                              receiver_thread, NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack), 
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  sender_thread, &receiver_pid, "sender");

    return 0;
}
