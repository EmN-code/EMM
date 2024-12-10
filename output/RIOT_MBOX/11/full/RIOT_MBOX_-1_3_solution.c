#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define INTERVAL (1U * US_PER_SEC)      // 1 second interval
#define QUEUE_SIZE (8)

static msg_t queue[QUEUE_SIZE];
static evtimer_msg_t evtimer;

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        xtimer_usleep(INTERVAL);
        msg.content.value++;
        printf("Sending message with value: %" PRIu32 "\n", msg.content.value);
        msg_send(&msg, thread_getpid());
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(
        sender_stack, sizeof(sender_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender_thread, NULL, "sender");

    printf("[INFO] Sender thread started with PID %d\n", sender_pid);

    evtimer_msg_init(&evtimer, INTERVAL, thread_getpid(), NULL);

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}