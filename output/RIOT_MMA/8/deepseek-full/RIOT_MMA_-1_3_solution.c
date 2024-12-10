
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void *sensor_thread(void *arg)
{
    (void)arg;
    mbox_t mbox;
    msg_t msg;
    msg_t msg_array[1];

    mbox_init(&mbox, msg_array, 1);

    while (1) {
        // Simulate reading the MMA8x5x sensor
        printf("Reading MMA8x5x sensor...\n");
        // Simulate a delay for sensor reading
        xtimer_sleep(5);
        msg.content.value = 1; // Dummy value
        msg_send(&msg, &mbox);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t sensor_pid = thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sensor_thread, NULL, "sensor_thread");

    mbox_t mbox;
    msg_t msg;
    msg_t msg_array[1];

    mbox_init(&mbox, msg_array, 1);

    while (1) {
        msg_receive(&msg);
        printf("Received sensor data: %d\n", msg.content.value);
    }

    return 0;
}
