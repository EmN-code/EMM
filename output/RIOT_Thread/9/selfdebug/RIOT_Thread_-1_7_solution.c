#include <stdio.h>
#include <inttypes.h>
#include <kernel_defines.h>
#include <msg.h>
#include <thread.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define MSG_QUEUE_SIZE 8

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

static msg_t msg_queue1[MSG_QUEUE_SIZE];
static msg_t msg_queue2[MSG_QUEUE_SIZE];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread1(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue1, MSG_QUEUE_SIZE);

    msg_t msg;
    unsigned int message_count = 0;

    while (1) {
        msg.content.value = message_count;
        msg_send(&msg, thread2_pid);
        printf("Thread 1 sent message: %" PRIu32 "\n", msg.content.value);
        message_count++;
        xtimer_sleep(1);
    }

    return NULL;
}

void *thread2(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue2, MSG_QUEUE_SIZE);

    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Thread 2 received message: %" PRIu32 "\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    puts("RIOT Messaging Example");

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                0, thread1, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                0, thread2, NULL, "thread2");

    return 0;
}