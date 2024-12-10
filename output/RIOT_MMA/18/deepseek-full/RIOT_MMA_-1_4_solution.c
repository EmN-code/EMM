
#include <stddef.h>
#include <msg.h>
#include <dht.h>
#include <timex.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <stdlib.h>
#include <MQTTClient.h>
#include <stdio.h>
#include <unistd.h>
#include <dht_params.h>

#define STACK_SIZE (1024)
#define MSG_QUEUE_SIZE (10)

static char stack[STACK_SIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

void sensor_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Sensor data read and processed.\n");
    }
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t sensor_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST, sensor_thread, NULL, "sensor");

    if (sensor_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create sensor thread.\n");
        return 1;
    }

    while (1) {
        msg_t msg;
        msg.type = 1;
        msg.content.ptr = NULL;
        msg_send(&msg, sensor_pid);
        sleep(5);
    }

    return 0;
}
