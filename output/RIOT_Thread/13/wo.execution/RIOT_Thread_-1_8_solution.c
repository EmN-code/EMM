#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "msg.h"
#include "mutex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"  // for sensor-related operation if needed
#include "arpa/inet.h"

#define THREAD_STACKSIZE (1024)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

// Message types
#define MESSAGE_TYPE_A 1
#define MESSAGE_TYPE_B 2

static char stack[THREAD_STACKSIZE];
static msg_t msg_queue[8];

void thread_function(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, 8);

    while (true) {
        msg_receive(&msg);  // Block until a message is received
        if (msg.type == MESSAGE_TYPE_A) {
            printf("Received message of type A with value: %ld\n", msg.content.value);
        }
        else if (msg.type == MESSAGE_TYPE_B) {
            printf("Received message of type B with pointer: %p\n", msg.content.ptr);
        }
    }
}

int main(void) {
    // Create a thread
    kernel_pid_t thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN,
                                            THREAD_CREATE_STACKTEST, thread_function,
                                            NULL, "receiver");

    msg_t msg;
 
    // Send message type A
    msg.content.value = 1234;
    msg.type = MESSAGE_TYPE_A;
    msg_send(&msg, thread_pid);

    // Send message type B
    msg.content.ptr = (void *)0xDEADBEEF;
    msg.type = MESSAGE_TYPE_B;
    msg_send(&msg, thread_pid);

    return 0;
}