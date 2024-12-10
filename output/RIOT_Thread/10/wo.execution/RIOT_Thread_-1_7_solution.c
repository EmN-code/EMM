#include <ztimer.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include <msg.h>
#include <event/thread.h>
#include <pthread.h>
#include <dht.h>
#include <paho_mqtt.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <mbox.h>

#define SENDER_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO      (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE_MAIN    1024

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

static msg_t sender_queue[8];
static msg_t receiver_queue[8];

void *receiver_thread(void *arg) {
    (void)arg;
    msg_init_queue(receiver_queue, 8);
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %ld from PID: %ld\n", (long)msg.content.value, (long)msg.sender_pid);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    msg_init_queue(sender_queue, 8);
    msg_t msg;
    msg.type = 0x1234;
    msg.content.value = 42;
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;

    while (1) {
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, 0, receiver_thread, NULL, "receiver");
    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0, sender_thread, &receiver_pid, "sender");

    (void)sender_pid;
    return 0;
}