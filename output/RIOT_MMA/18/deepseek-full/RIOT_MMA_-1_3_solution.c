
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "mbox.h"
#include "xtimer.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "mma8x5x_params.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void *sensor_thread(void *arg)
{
    (void)arg;
    msg_t msg, reply;
    mbox_t mbox;
    evtimer_mbox_t et;
    evtimer_mbox_event_t event;

    mbox_init(&mbox, _main_msg_queue, MAIN_QUEUE_SIZE);
    evtimer_mbox_init(&et, &mbox);

    while (1) {
        msg_receive(&msg);
        printf("Reading sensor data...\n");
        // Simulate sensor reading
        ztimer_sleep(ZTIMER_MSEC, 100);
        printf("Sensor data read complete.\n");

        // Send a reply message
        reply.content.value = 0;
        msg_reply(&msg, &reply);
    }

    return NULL;
}

int main(void)
{
    msg_t msg;
    kernel_pid_t sensor_pid = thread_create(
        stack, sizeof(stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sensor_thread, NULL, "sensor_thread");

    evtimer_mbox_event_t event;
    evtimer_mbox_t et;
    mbox_t mbox;

    mbox_init(&mbox, _main_msg_queue, MAIN_QUEUE_SIZE);
    evtimer_mbox_init(&et, &mbox);

    while (1) {
        // Schedule a sensor read event every 5 seconds
        event.event.offset = timex_set(5, 0);
        event.event.callback = (evtimer_callback_t)mbox_event_callback;
        event.mbox = &mbox;
        evtimer_add(&et, &event.event);

        // Wait for the sensor thread to process the event
        msg_receive(&msg);
        printf("Main thread received reply.\n");
    }

    return 0;
}
