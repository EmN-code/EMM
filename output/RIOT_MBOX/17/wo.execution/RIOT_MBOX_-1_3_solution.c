#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "MQTTClient.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "time_units.h"

#define SENDER_THREAD_STACKSIZE   (THREAD_STACKSIZE_MAIN)
#define SENDER_THREAD_PRIORITY    (THREAD_PRIORITY_MAIN - 1)
#define MSG_QUEUE_SIZE            (8)

static char sender_thread_stack[SENDER_THREAD_STACKSIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];
kernel_pid_t sender_pid;

void *sender_thread(void *arg) {
    (void) arg;
    msg_t message;
    int msg_count = 0;

    while(1) {
        xtimer_sleep(1);
        message.type = 0xABCD;
        message.content.value = msg_count++;
        msg_send(&message, thread_getpid());  // Sends to itself for demonstration
        printf("Sent message with count: %d\n", message.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    sender_pid = thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                               SENDER_THREAD_PRIORITY, 0,
                               sender_thread, NULL, "sender_thread");

    msg_t received_msg;

    while (1) {
        if (msg_receive(&received_msg) == 1) {
            printf("Received message with count: %d\n", received_msg.content.value);
        }
    }

    return 0;
}