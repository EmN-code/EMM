#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <netinet/in.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>

void *event_periodic(void *arg) {
    (void)arg;
    while (1) {
        // The callback functionality: to be executed every second
        printf("Callback executed.\n");
        sleep(1);  // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    // Definitions for configuring the thread
    static char thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, 0, event_periodic, NULL, "event_thread");
    
    // Keep the main thread alive
    while (1) {
        sleep(10);  // Sleep for 10 seconds
    }
    return 0;
}