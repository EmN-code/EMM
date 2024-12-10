#include "evtimer_msg.h"
#include "time_units.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"

#include <stdio.h>
#include <stdlib.h>

// Custom function to simulate MQTT connection and publishing
void connect_and_publish_mqtt(const char *address, int port, const char *topic, const char *message) {
    // Simulating a connection
    printf("Connecting to MQTT broker at %s:%d\n", address, port);
    // Simulating publishing a message
    printf("Publishing message to topic '%s': %s\n", topic, message);
}

// Thread task function to simulate the work each 5 seconds
void *publisher_thread(void *arg) {
    (void)arg;
    while (1) {
        connect_and_publish_mqtt("47.102.103.1", 1883, "state", "work or not work");
        sleep(5);  // Retry every 5 seconds
    }
    return NULL;
}

int main(void) {
    char thread_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher_thread, NULL, "publisher");

    // Keeping main thread alive
    while (1) {
        // Implement a low-power sleep or power management function if necessary
    }

    return 0;
}