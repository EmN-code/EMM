# This example demonstrates setting up a message box in RIOT OS using ESP32
# and repeatedly sending events. We include a variety of RIOT headers for the
# components needed to manage threads, timers and messages.

# Include necessary header files
#include <xtimer.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <net/sock/util.h>
#include <string.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <stdio.h>
#include <msg.h>
#include <stdlib.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <mma8x5x_params.h>
#include <net/emcute.h>
#include <esp_wifi.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <ztimer.h>
#include <mutex.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <mma8x5x.h>
#include <net/sock/udp.h>
#include <thread.h>
#include <arpa/inet.h>
#include <timex.h>
#include <esp_system.h>
#include <dht.h>
#include <sys/socket.h>
#include <net/gcoap.h>

# Define stack size for threads
#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)

# Define message queue size
#define QUEUE_SIZE (8U)

# Define a message box for communication
static msg_t _queue[QUEUE_SIZE];

# Define thread function
void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Message received: %d\n", msg.content.value);
    }
    return NULL;
}

# Main function to initiate message box
int main(void) {
    # Initiate the message queue for current thread
    msg_init_queue(_queue, QUEUE_SIZE);

    # Create a thread for handling messages
    char thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_handler, NULL, "message_handler");

    # Start sending events
    msg_t msg;
    while (1) {
        msg.content.value = 42;  # Example message content
        msg_send(&msg, thread_getpid());
        xtimer_sleep(1);  # Sleep for 1 second
    }

    return 0;
}