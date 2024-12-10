#include <ztimer.h>         // Timer library for RIOT
#include <sys/socket.h>    // Socket programming (not used but required per your request)
#include <stddef.h>        // Standard definitions like NULL
#include <stdatomic.h>     // Atomic operations
#include <stdlib.h>        // Standard library definitions
#include <stdio.h>         // Standard Input Output
#include <stdbool.h>       // Boolean types
#include <paho_mqtt.h>     // MQTT client library
#include <netinet/in.h>    // Internet address family
#include <net/sock/util.h> // Socket utilities
#include <net/sock/udp.h>  // UDP socket
#include <net/gcoap.h>     // CoAP
#include <net/emcute.h>    // Lightweight MQTT
#include <mutex.h>         // Mutex support
#include <mma8x5x_params.h>// MMA8x5x parameters (hypothetical)
#include <mma8x5x_regs.h>  // MMA8x5x registers (hypothetical)
#include <mma8x5x.h>       // MMA8x5x driver (hypothetical)
#include <kernel_defines.h>// Kernel definitions
#include <evtimer_mbox.h>  // Event Timer MBox API (hypothetical)
#include <evtimer_msg.h>   // Event Timer Msg API (hypothetical)
#include <event/thread.h>  // Event Threads
#include <esp_system.h>    // ESP32 system functions
#include <arpa/inet.h>     // IP address manipulation
#include <dht_params.h>    // DHT sensor parameters
#include <dht.h>           // DHT sensors library
#include <mbox.h>          // Message box helpers
#include <unistd.h>        // UNIX standard definitions
#include <timex.h>         // Time definitions
#include <xtimer.h>        // Cross-striped timer
#include <time_units.h>    // Time units

#define THREAD_PRIORITY_MAIN  (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_MAIN)

// Define a stack size
#define THREAD_STACKSIZE      (KERNEL_CONF_STACKSIZE_DEFAULT)

// Create stacks for threads
static char stack_one[THREAD_STACKSIZE];
static char stack_two[THREAD_STACKSIZE];

// Variables for threads
kernel_pid_t pid_one;
kernel_pid_t pid_two;

msg_t msg_queue_one[8];
msg_t msg_queue_two[8];

// Function declaration for the thread that sends messages
void *thread_one(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        printf("Thread one sending message...\n");
        msg.content.value = 42;
        msg_send(&msg, pid_two);
        xtimer_sleep(1);
    }
    return NULL;
}

// Function declaration for the receiver thread
void *thread_two(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        if (msg_receive(&msg) > 0) {
            printf("Thread two received message with value: %" PRIu32 "\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    // Initialize message queues
    msg_init_queue(msg_queue_one, sizeof(msg_queue_one) / sizeof(msg_t));
    msg_init_queue(msg_queue_two, sizeof(msg_queue_two) / sizeof(msg_t));

    // Create threads
    pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, thread_one, NULL, "thread_one");
    pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_two, NULL, "thread_two");

    return 0;
}