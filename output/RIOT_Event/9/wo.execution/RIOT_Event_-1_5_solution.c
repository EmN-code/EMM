#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "timex.h"
#include "paho_mqtt.h"
#include "kernel_defines.h"
#include "MQTTClient.h"

// Define stack size and priority for the thread
#define THREAD_STACKSIZE_MAIN (1024)
#define THREAD_PRIORITY_MAIN (5)

// Thread stack
static char stack[THREAD_STACKSIZE_MAIN];

// Callback function to be called every second
void callback_function(void *arg) {
    (void)arg;
    printf("Callback called\n");
}

// Thread function
void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        // Call the callback function every second
        callback_function(NULL);
        sleep(1);
    }
    return NULL;
}

// Main function
int main(void) {
    // Create a new thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, thread_handler, NULL, "callback_thread");

    // Keep the main function running indefinitely
    while(1) {
        sleep(1);
    }
    return 0;
}