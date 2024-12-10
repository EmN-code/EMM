#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "msg.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_IDLE + 1)
#define THREAD_STACKSIZE_MAIN   THREAD_STACKSIZE_DEFAULT
#define THREAD_STACKSIZE        (1024)
#define SENDER_MSG_TYPE         (0x1234)

char sender_stack[THREAD_STACKSIZE];
char receiver_stack[THREAD_STACKSIZE];

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

msg_t queue[8];
msg_t msg;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t send_msg;
    while (1) {
        send_msg.type = SENDER_MSG_TYPE;
        send_msg.content.value = 42;  // Sample message content
        int sent = msg_send(&send_msg, receiver_pid);
        if (sent == 1) {
            printf("Message sent to receiver\n");
        } else {
            printf("Failed to send message\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, 8);
    while (1) {
        msg_receive(&msg);
        if (msg.type == SENDER_MSG_TYPE) {
            printf("Received message with value: %" PRIu32 "\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_MAIN - 1, 0,
                                 receiver_thread, NULL, "receiver");

    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               THREAD_PRIORITY_MAIN, 0,
                               sender_thread, NULL, "sender");

    while (1) {
        thread_yield();
    }

    return 0;
}