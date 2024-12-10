#include <ztimer.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <paho_mqtt.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <timex.h>
#include <unistd.h>
#include <MQTTClient.h>
#include <time_units.h>
#include <thread.h>
#include <event/thread.h>

#define THREAD_STACKSIZE_DEFAULT (1024)
#define THREAD_PRIORITY_DEFAULT (THREAD_PRIORITY_MAIN - 1)

static char thread_stack_buf[THREAD_STACKSIZE_DEFAULT];

static void *thread_handler(void *arg) {
    msg_t msg;
    (void)arg;
    while (1) {
        // Code for thread task
        msg_receive(&msg);
        // Process message here
    }
    return NULL;
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    thread_create(thread_stack_buf,
                  sizeof(thread_stack_buf),
                  THREAD_PRIORITY_DEFAULT,
                  0,
                  thread_handler,
                  NULL,
                  "thread_handler");
    
    msg_t msg;
    msg.type = 1; // Specify message type
    
    while (1) {
        // Send a message to the thread
        msg_send(&msg, thread_getpid());
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}