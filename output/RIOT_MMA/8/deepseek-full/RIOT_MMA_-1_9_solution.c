
#include "esp_system.h"
#include "net/gcoap.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "mutex.h"
#include "msg.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "mbox.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "thread.h"
#include "unistd.h"
#include "stdatomic.h"
#include "net/sock/udp.h"

#define STACK_SIZE (1024)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MSG_QUEUE_SIZE (8)

static char stack[STACK_SIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

void sensor_thread(void *arg)
{
    (void)arg;

    while (1) {
        // Simulate reading the MMA8x5x sensor
        printf("Reading MMA8x5x sensor...\n");

        // Simulate a delay of 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create the sensor thread
    thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST, sensor_thread, NULL, "sensor_thread");

    // Main loop
    while (1) {
        // Simulate other tasks
        printf("Main thread running...\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}
