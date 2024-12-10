
#include "kernel_defines.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "unistd.h"
#include "msg.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "stddef.h"
#include "stdbool.h"
#include "dht.h"
#include "stdatomic.h"
#include "dht_params.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void accelerometer_read_thread(void *arg)
{
    (void)arg;
    while (1) {
        // Simulate reading accelerometer data
        printf("Reading accelerometer data...\n");
        // Simulate a 5-second delay
        xtimer_sleep(5);
    }
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Initialize the accelerometer (simulated)
    printf("Initializing accelerometer...\n");

    // Create a thread for reading accelerometer data
    thread_create(
        stack, sizeof(stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        accelerometer_read_thread,
        NULL, "accelerometer_read");

    return 0;
}
