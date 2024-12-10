/* This implementation shows inter-thread communication using msg_t in RIOT OS
   on an ESP32 without the use of certain prohibited headers/API elements. */

#include "ztimer.h"
#include "xtimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "thread.h"
#include "msg.h"

#define SENDER_THREAD_PRIO   (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIO (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE     (THREAD_STACKSIZE_DEFAULT)

char sender_stack[THREAD_STACKSIZE];
char receiver_stack[THREAD_STACKSIZE];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    kernel_pid_t receiver_pid = thread_getpid(); // Assumed to be receiver thread's PID.
    int cnt = 0;
    
    while (1) {
        msg.content.value = cnt;
        msg.type = 0x1234;
        printf("Sender: Sending message %d\n", cnt);
        msg_send(&msg, receiver_pid);
        cnt++;
        xtimer_sleep(1); // Sleep for a second before sending next message.
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(&msg, 1); // Initialize message queue
    
    while (1) {
        msg_receive(&msg); // Blocking receive
        printf("Receiver: Received message with value %lu\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t sender_pid, receiver_pid;
    
    receiver_pid = thread_create(receiver_stack, THREAD_STACKSIZE, RECEIVER_THREAD_PRIO, 0, receiver, NULL, "receiver");
    if (receiver_pid <= KERNEL_PID_UNDEF) {
        puts("Error: unable to start receiver thread");
        return 1;
    }
    
    sender_pid = thread_create(sender_stack, THREAD_STACKSIZE, SENDER_THREAD_PRIO, 0, sender, NULL, "sender");
    if (sender_pid <= KERNEL_PID_UNDEF) {
        puts("Error: unable to start sender thread");
        return 1;
    }
    
    return 0;
}