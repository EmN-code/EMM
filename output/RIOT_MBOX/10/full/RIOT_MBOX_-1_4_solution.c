#include <xtimer.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <event/thread.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <stdio.h>
#include <msg.h>
#include <stdlib.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <ztimer.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <mma8x5x.h>
#include <thread.h>
#include <timex.h>

#define QUEUE_SIZE       (8) // Must be a power of two
#define SENDER_MSG_TYPE  (100)

static msg_t msg_queue[QUEUE_SIZE];
static kernel_pid_t main_pid;

void *msg_sender(void *arg) {
    (void)arg; // unused variable
    msg_t msg;
    msg.type = SENDER_MSG_TYPE;
    while(1) {
        xtimer_sleep(2); // Sleep for 2 seconds
        msg_send(&msg, main_pid);
    }
    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, QUEUE_SIZE);

    // Store main thread PID
    main_pid = thread_getpid();

    // Create a new thread for sending messages
    char sender_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, msg_sender, NULL, "msg_sender");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        if (msg.type == SENDER_MSG_TYPE) {
            puts("Message received from sender!");
        }
    }
    return 0;