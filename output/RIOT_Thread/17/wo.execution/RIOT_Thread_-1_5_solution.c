#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "unistd.h"
#include "thread.h"
#include "msg.h"

#define THREAD_STACKSIZE_MAIN  (THREAD_STACKSIZE_DEFAULT)
#define SENDER_PRIO            (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO          (THREAD_PRIORITY_MAIN - 2)
#define MAIN_QUEUE_SIZE        (8)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[MAIN_QUEUE_SIZE];

void *receiver(void *arg) {
    msg_t msg;
    (void)arg;
    msg_init_queue(msg_queue, MAIN_QUEUE_SIZE);
    
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received message with type: %u and value: %u\n", msg.type, msg.content.value);
        }
    }
    return NULL;
}

void *sender(void *arg) {
    (void)arg;
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;

    while (1) {
        msg_t msg;
        msg.type = 0x1234;
        msg.content.value = rand();
        
        // Sending message
        if (msg_send(&msg, receiver_pid) == 1) {
            printf("Sent message with type: %u and value: %u\n", msg.type, msg.content.value);
        }
        ztimer_sleep(ZTIMER_USEC, 1000000); // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, 0, receiver, NULL, "receiver");
    printf("Receiver thread started\n");

    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0, sender, &receiver_pid, "sender");
    printf("Sender thread started\n");

    return 0;
}
