#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "mbox.h"
#include "msg.h"
#include "thread.h"
#include "unistd.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "event/thread.h"
#include "dht_params.h"

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_HELPER  (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE_MAIN          (THREAD_STACKSIZE_DEFAULT)
#define STACKSIZE_HELPER        (THREAD_STACKSIZE_DEFAULT)

static char stack_main[STACKSIZE_MAIN];
static char stack_helper[STACKSIZE_HELPER];

static msg_t msg_queue[8];

void *helper_thread(void *arg) {
    msg_t msg;
    (void)arg;
    // Initialize message queue for this thread
    msg_init_queue(msg_queue, 8);
    
    while(1) {
        // Wait to receive a message
        msg_receive(&msg);
        printf("Received message of type: %u\n", msg.type);
        // Implement message handling logic
        // For instance, reply back to the sender
        msg_t reply_msg;
        reply_msg.type = msg.type + 1;
        msg_reply(&msg, &reply_msg);
    }
    return NULL;
}

int main(void)
{
    msg_t msg;
    msg_t reply;
    kernel_pid_t pid;

    xtimer_sleep(1); // Small delay to start threads in order
    
    // Create helper_thread
    pid = thread_create(stack_helper, sizeof(stack_helper), THREAD_PRIORITY_HELPER,
                        THREAD_CREATE_STACKTEST, helper_thread, NULL, "helper");

    printf("Helper thread PID: %d\n", pid);

    // Main thread sending a message
    msg.type = 42; // Arbitrary message type
    printf("Main thread sending message of type: %u\n", msg.type);
    msg_send_receive(&msg, &reply, pid);
    printf("Main thread received reply of type: %u\n", reply.type);

    return 0;
}