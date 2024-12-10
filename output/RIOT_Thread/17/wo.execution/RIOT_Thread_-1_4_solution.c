#include "xtimer.h"
#include "ztimer.h"
#include "thread.h"
#include "time_units.h"
#include "msg.h"

#define SENDER_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO      (SENDER_PRIO - 1)
#define THREAD_STACKSIZE_MAIN    THREAD_STACKSIZE_DEFAULT
#define MESSAGE_QUEUE_SIZE       (4U)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

static msg_t queue[MESSAGE_QUEUE_SIZE];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg.content.value = (uint32_t) esp_timer_get_time();
        printf("Sender: Sending message with value: %lu\n", msg.content.value);
        msg_send(&msg, thread_getpid());
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(queue, MESSAGE_QUEUE_SIZE);

    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Receiver: Received message with value: %lu\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    puts("RIOT OS Message Passing Example");

    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0,
                  sender, NULL, "sender");

    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, 0,
                  receiver, NULL, "receiver");

    return 0;
}