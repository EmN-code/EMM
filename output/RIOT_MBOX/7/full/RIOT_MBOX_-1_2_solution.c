#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

#define SENDER_PRIO       (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO     (THREAD_PRIORITY_MAIN - 2)
#define SENDER_STACKSIZE  (THREAD_STACKSIZE_MAIN + 256)
#define RECEIVER_STACKSIZE (THREAD_STACKSIZE_MAIN + 256)

#define INTERVAL_SEC      (1U)
#define MSG_QUEUE_SIZE    (8U)

static char sender_thread_stack[SENDER_STACKSIZE];
static char receiver_thread_stack[RECEIVER_STACKSIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

static kernel_pid_t sender_pid;
static kernel_pid_t receiver_pid;

void *sender(void *arg) {
    (void) arg;
    msg_t msg;
    msg_t reply;
    
    while (1) {
        xtimer_sleep(INTERVAL_SEC);
        msg_send_receive(&msg, &reply, receiver_pid);
    }
    return NULL;
}

void *receiver(void *arg) {
    (void) arg;
    msg_t msg;
    
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    
    while (1) {
        msg_receive(&msg);
        // Process the received message
    }
    return NULL;
}

int main(void) {
    sender_pid = thread_create(sender_thread_stack, SENDER_STACKSIZE, SENDER_PRIO, THREAD_CREATE_STACKTEST, sender, NULL, "sender");
    receiver_pid = thread_create(receiver_thread_stack, RECEIVER_STACKSIZE, RECEIVER_PRIO, THREAD_CREATE_STACKTEST, receiver, NULL, "receiver");

    // Main thread can perform other tasks or go to sleep
    while(1) {
        xtimer_sleep(10);
    }
    return 0;
}