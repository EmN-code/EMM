
#include "esp_system.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "mutex.h"
#include "msg.h"
#include "stdlib.h"
#include "stdio.h"
#include "thread.h"

#define STACKSIZE 1024
#define PRIORITY 5
#define MSG_QUEUE_SIZE 8

static char stack[STACKSIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

void accelerometer_task(void *arg)
{
    (void)arg;

    while (1) {
        printf("Simulated accelerometer reading every 5 seconds\n");
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_create(stack, STACKSIZE, PRIORITY, 0, accelerometer_task, NULL, "accelerometer_task");

    return 0;
}
