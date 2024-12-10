#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <msg.h>
#include <mbox.h>
#include <MQTTClient.h>

#define PERIOD 1000000 // 1 second in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];

static int periodic_task(void *arg) {
    (void)arg;
    while (1) {
        printf("Callback function executed\n");
        xtimer_usleep(PERIOD);
    }
    return 0;
}

int main(void) {
    // Initialize and start a background thread with periodic task
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, periodic_task, NULL, "periodic");

    // Block main thread indefinitely
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_t m;
        msg_receive(&m);
    }

    return 0;
}