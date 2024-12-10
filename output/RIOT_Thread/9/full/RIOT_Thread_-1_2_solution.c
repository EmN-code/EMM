#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "msg.h"

#define THREAD_PRIORITY_MAIN  (THREAD_PRIORITY_MAIN - 1)
#define MSG_QUEUE_SIZE       (8)
#define SENDER_MSG_TYPE_1    (1)
#define SENDER_MSG_TYPE_2    (2)

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message of type: %u, with value: %" PRIu32 "\n", msg.type, msg.content.value);
    }

    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;
    msg_t msg;

    while (1) {
        msg.type = SENDER_MSG_TYPE_1;
        msg.content.value = esp_random();
        msg_send(&msg, receiver_pid);

        msg.type = SENDER_MSG_TYPE_2;
        msg.content.value = esp_random();
        msg_send(&msg, receiver_pid);

        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                  THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                                  receiver_thread, NULL, "receiver");

    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create receiver thread\n");
        return 1;
    }

    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                               sender_thread, &receiver_pid, "sender");

    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create sender thread\n");
        return 1;
    }

    return 0;
}
