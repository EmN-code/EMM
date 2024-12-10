
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <mbox.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <MQTTClient.h>

#define STACKSIZE 1024
#define MSG_QUEUE_SIZE 8

typedef struct {
    kernel_pid_t sender_pid;
    uint16_t type;
    union {
        void *ptr;
        uint32_t value;
    } content;
} msg_t;

msg_t msg_queue[MSG_QUEUE_SIZE];

void low_prio_thread(void)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message with value: %lu\n", (unsigned long)msg.content.value);
    }
}

void high_prio_thread(void)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message with value: %lu\n", (unsigned long)msg.content.value);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t low_prio_pid = thread_create(NULL, STACKSIZE, THREAD_PRIORITY_MAIN - 1, 0, (thread_task_func_t)low_prio_thread, NULL, "low_prio_thread");
    kernel_pid_t high_prio_pid = thread_create(NULL, STACKSIZE, THREAD_PRIORITY_MAIN - 2, 0, (thread_task_func_t)high_prio_thread, NULL, "high_prio_thread");

    msg_t msg;
    msg.type = 1;
    msg.content.value = 123;

    while (1) {
        msg_send(&msg, low_prio_pid);
        msg_send(&msg, high_prio_pid);
        sleep(1);
    }

    return 0;
}
