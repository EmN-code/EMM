#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "msg.h"
#include "thread.h"
#include "mbox.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "time_units.h"

#define BUFFER_SIZE     (64U)
#define PRIO_HIGH       (5)
#define PRIO_LOW        (7)
#define MAIN_QUEUE_SIZE (8)

static char stack_low[THREAD_STACKSIZE_MAIN];
static char stack_high[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[MAIN_QUEUE_SIZE];

static void *low_prio(void *arg) {
    (void)arg;
    msg_t msg;
    msg_send_receive(&msg, &msg, thread_getpid());
    printf("Low priority thread received message from PID %d\n", msg.sender_pid);
    return NULL;
}

static void *high_prio(void *arg) {
    (void)arg;
    msg_t msg;
    msg_send_receive(&msg, &msg, thread_getpid());
    printf("High priority thread received message from PID %d\n", msg.sender_pid);
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t low_pid = thread_create(stack_low, sizeof(stack_low), PRIO_LOW, 0, high_prio, NULL, "low_prio");
    kernel_pid_t high_pid = thread_create(stack_high, sizeof(stack_high), PRIO_HIGH, 0, low_prio, NULL, "high_prio");

    // Send messages from the main thread
    msg_t msg;
    msg.type = 0xCAFE;
    msg_send(&msg, low_pid);
    msg_send(&msg, high_pid);

    return 0;
}