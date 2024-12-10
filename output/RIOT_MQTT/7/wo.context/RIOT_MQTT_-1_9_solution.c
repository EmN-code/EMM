#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "net/mqttsn.h" // Assuming this is the available MQTT library in RIOT for ESP32
#include "thread.h"
#include "xtimer.h"
#include "random.h"

// Ensure last inclusion of MQTTClient.h if necessary
//#include "mqtt/MQTTClient.h"

#ifndef MQTT_BROKER_IP
#define MQTT_BROKER_IP "47.102.103.1"
#endif
#ifndef MQTT_BROKER_PORT
#define MQTT_BROKER_PORT 1883
#endif

static kernel_pid_t mqtt_thread_pid;
static char mqtt_thread_stack[THREAD_STACKSIZE_MAIN];

static void *mqtt_thread(void *arg) {
    (void)arg;
    while (1) {
        // Simulating MQTT publish using MQTT-SN and checking work status
        int work_state = random_uint32() % 2; // Random work state (0 or 1)
        printf("Publishing MQTT state: %s\n", work_state == 1 ? "work" : "not work");
        
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    printf("Starting MQTT example for ESP32 on RIOT\n");

    // Initializing thread for MQTT
    mqtt_thread_pid = thread_create(mqtt_thread_stack, sizeof(mqtt_thread_stack),
                                    THREAD_PRIORITY_MAIN - 1, 0,
                                    mqtt_thread, NULL, "mqtt_thread");
    assert(mqtt_thread_pid > KERNEL_PID_UNDEF);

    // Main thread's execution
    while (1) {
        // The main thread can execute other tasks
        xtimer_sleep(1);
    }

    return 0;
}