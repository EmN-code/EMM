
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "dht.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "stdlib.h"
#include "MQTTClient.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "mma8x5x_params.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MSG_QUEUE_SIZE (8)

static char stack[STACKSIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

void *sensor_thread(void *arg)
{
    (void)arg;

    while (1) {
        // Simulate reading MMA8x5x sensor data
        printf("Simulating MMA8x5x sensor reading...\n");

        // Simulate a 5-second delay
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_create(stack, sizeof(stack), PRIORITY, 0, sensor_thread, NULL, "sensor_thread");

    return 0;
}
