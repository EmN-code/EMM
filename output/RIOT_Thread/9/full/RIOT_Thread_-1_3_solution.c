#include <ztimer.h>
#include <xtimer.h>  
#include <timex.h>  
#include <time_units.h>
#include <thread.h>
#include <msg.h>
#include <mbox.h>
#include <MQTTClient.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <mma8x5x_regs.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

#define MSG_QUEUE_SIZE           (8)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void) arg;
    kernel_pid_t receiver_pid = (kernel_pid_t) arg;

    msg_t msg;
    msg.type = 0x42;
    msg.content.value = 100;

    while (1) {
        if (msg_send(&msg, receiver_pid) <= 0) {
            puts("Failed to send message");
        } else {
            puts("Message sent");
        }
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void) arg;
    msg_t msg;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        if (msg_receive(&msg) > 0) {
            printf("Received message with type: %u and value: %u\n", msg.type, msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid;

    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_MAIN - 1,
                                 THREAD_CREATE_STACKTEST,
                                 receiver_thread, NULL, "receiver");

    if (receiver_pid <= 0) {
        puts("Failed to create receiver thread");
        return 1;
    }

    if (thread_create(sender_stack, sizeof(sender_stack),
                      THREAD_PRIORITY_MAIN,
                      THREAD_CREATE_STACKTEST,
                      sender_thread, (void*)receiver_pid, "sender") <= 0) {
        puts("Failed to create sender thread");
        return 1;
    }

    return 0;
}