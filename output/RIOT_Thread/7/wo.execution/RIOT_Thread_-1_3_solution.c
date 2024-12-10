#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "msg.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define SENDER_THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define RECEIVER_THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define HIGH_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define LOW_PRIORITY  (THREAD_PRIORITY_MAIN + 1)

static char sender_stack[SENDER_THREAD_STACKSIZE];
static char receiver_stack[RECEIVER_THREAD_STACKSIZE];
msg_t msg_queue[8];

void *receiver_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, 8);
    
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received message from thread %d with type: %u\n", msg.sender_pid, msg.type);
        }
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    kernel_pid_t receiver_pid = *((kernel_pid_t*)arg);
    msg_t msg;

    while (1) {
        msg.type = (uint16_t)(esp_random() % 100);  // Generating a random message type
        msg.content.value = esp_random();
        msg.sender_pid = thread_get_id();
        msg_send(&msg, receiver_pid);

        ztimer_sleep(ZTIMER_MSEC, 1000);  // Sleep for 1 second before sending next message
    }
    return NULL;
}

int main(void) {
    printf("Starting message passing example with ESP32 and RIOT\n");

    kernel_pid_t receiver_pid = thread_create(receiver_stack, RECEIVER_THREAD_STACKSIZE,
                                              HIGH_PRIORITY, THREAD_CREATE_STACKTEST,
                                              receiver_thread, NULL, "receiver");

    thread_create(sender_stack, SENDER_THREAD_STACKSIZE,
                  LOW_PRIORITY, THREAD_CREATE_STACKTEST,
                  sender_thread, &receiver_pid, "sender");

    return 0;
}