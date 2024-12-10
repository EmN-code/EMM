#include <ztimer.h>
#include <xtimer.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <kernel_defines.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <msg.h>
#include <thread.h>

#define STACKSIZE 1024
char thread_stack[STACKSIZE];

void receiver_thread(void *arg) {
    (void)arg;
    evtimer_mbox_event_t event;
    msg_t msg;

    while (1) {
        if (evtimer_msg_receive(&msg)) {
            event = evtimer_event_get(&msg);
            printf("Received message: %d, from sender with PID: %d\n", event.msg.content.value, event.msg.sender_pid);
        }
        xtimer_sleep(1);
    }
}

void sender_thread(void *arg) {
    kernel_pid_t receiver_pid = *((kernel_pid_t *)arg);
    msg_t msg;
    evtimer_mbox_event_t event;

    msg.type = 0x1234; // Message type identifier

    int count = 0;
    while (1) {
        msg.content.value = count++;
        msg_send(&msg, receiver_pid);
        printf("Sent message: %d to receiver with PID: %d\n", msg.content.value, receiver_pid);
        ztimer_sleep(ZTIMER_USEC, 1000000); // Sleep for 1 second
    }
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(
        thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Error: Failed to create receiver thread\n");
        return 1;
    }

    sender_thread((void *)&receiver_pid);
    return 0;
}
