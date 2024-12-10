#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stddef.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "habt.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "sys/socket.h"
#include "time_units.h"
#include "timex.h"
#include "unistd.h"
#include "paho_mqtt.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "dht_params.h"
#include "dht.h"

#define INTERVAL (1U * US_PER_SEC)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static kernel_pid_t thread_pid;
static char stack[THREAD_STACKSIZE_MAIN];

// Message queue for the thread
static msg_t msg_queue[8];  // Ensure the size is a power of two per documentation

void *thread_handler(void *arg) {
    (void) arg;
    msg_init_queue(msg_queue, 8);
    msg_t msg;

    while (1) {
        if (ztimer_sleep(ZTIMER_WAIT, INTERVAL) < 0) {
            printf("Error in timer sleep!\n");
            continue;
        }

        // Receiving message example
        msg_receive(&msg);
        printf("Received message type: %u\n", msg.type);

        // Logic to handle the message should be here
    }
    return NULL;
}

int main(void) {
    puts("RIOT: ESP32 Message Box Example");

    // Create a thread
    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY, 0,
                               thread_handler, NULL, "msg_thread");

    if (thread_pid <= KERNEL_PID_UNDEF) {
        puts("Failed to create thread\n");
        return 1;
    }

    // Main loop doing other tasks
    while (1) {
        // Send a message as an example
        msg_t msg;
        msg.type = 1;  // Example type
        msg.content.value = 42;  // Example content
        printf("Sending message type: %u\n", msg.type);

        msg_send(&msg, thread_pid);
    }

    return 0;
}