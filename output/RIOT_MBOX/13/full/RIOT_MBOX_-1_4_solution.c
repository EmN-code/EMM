#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <kernel_defines.h>
#include <thread.h>
#include <paho_mqtt.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <time_units.h>
#include <MQTTClient.h>

#define INTERVAL (1U * US_PER_SEC)

static msg_t main_msg_queue[8];

void *event_thread(void *arg) {
    (void)arg;
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        puts("Event received");
    }
    return NULL;
}

int main(void) {
    puts("RIOT message box example");

    // Initialize message queue
    msg_init_queue(main_msg_queue, ARRAY_SIZE(main_msg_queue));

    // Create an event thread
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, event_thread, NULL, "event_thread");

    while (1) {
        msg_t msg;
        msg.content.value = 42;  // Example event content

        // Sending a message to the message queue
        msg_send(&msg, thread_getpid_of(event_thread));

        // Wait for the next event to be sent
        ztimer_sleep(ZTIMER_SEC, INTERVAL);
    }

    return 0;
}