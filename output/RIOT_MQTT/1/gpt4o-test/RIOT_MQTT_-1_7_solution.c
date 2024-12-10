#include "ztimer.h"
#include "thread.h"
#include "net/sock/util.h"
#include "architecture.h"
#include "assert.h"
#include "sys/socket.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "stddef.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "freertos/FreeRTOS.h"
#include "arpa/inet.h"
#include "evtimer_msg.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdlib.h"
#include "mma8x5x_regs.h"
#include "stdio.h"
#include "timex.h"
#include "esp_wifi.h"
#include "net/emcute.h"
#include "esp_system.h"
#include "msg.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "dht_params.h"
#include "dht.h"
#include "kernel_defines.h"
#include "evtimer_mbox.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "mbox.h"
#include "mutex.h"
#include "freertos/task.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "RIOT_MQTT_Client"
#define TOPIC "device/state"
#define PUB_INTERVAL (5LU * 1000LU * 1000LU) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_MAIN];

void *mqtt_thread(void *arg) {
    (void)arg; // Argument is unused

    while (1) {
        // Simulate publishing "work or not work" status
        printf("Publishing message: work\n");

        // Pause for the specified interval
        xtimer_usleep(PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    kernel_pid_t mqtt_thread_pid = thread_create(
        stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
        0, mqtt_thread, NULL, "mqtt_thread");

    assert(mqtt_thread_pid > KERNEL_PID_UNDEF);

    // Main thread can perform other tasks or sleep indefinitely
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}