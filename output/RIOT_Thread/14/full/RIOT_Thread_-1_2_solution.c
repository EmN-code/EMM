#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define THREAD_PRIORITY_1   (THREAD_PRIORITY_MAIN + 1)
#define THREAD_PRIORITY_2   (THREAD_PRIORITY_MAIN + 2)
#define MSG_QUEUE_SIZE      (8)
#define STACKSIZE           (THREAD_STACKSIZE_MAIN)

msg_t msg_queue[MSG_QUEUE_SIZE];
kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

char stack1[STACKSIZE];
char stack2[STACKSIZE];

void *thread1_entry(void *arg);
void *thread2_entry(void *arg);

void *thread1_entry(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    while (1) {
        printf("Sending message from thread1 to thread2\n");
        if (msg_send(&msg, thread2_pid) <= 0) {
            printf("Failed to send message\n");
        }
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread2_entry(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message in thread2 with value: %ld\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_1, 0,
                                thread1_entry, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_2, 0,
                                thread2_entry, NULL, "thread2");

    return 0;
}