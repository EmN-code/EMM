# Include the necessary RIOT headers in descending alphabetical order
#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <dht_params.h>
#include <dht.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <thread.h>
#include <timex.h>
#include <net/gcoap.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <MQTTClient.h>
#include <mbox.h>
#include <msg.h>
#include <unistd.h>
#include <stdatomic.h>
#include <paho_mqtt.h>

// Define message type
#define MESSAGE_TYPE 1

// This is a periodic message sender that will send messages to a message box
void *msg_sender(void *arg) {
    (void)arg;
    msg_t msg;
    
    while (1) {
        msg.type = MESSAGE_TYPE;
        msg.content.value = rand();  // Random data for demonstration
        
        // Send the message at intervals
        msg_send(&msg, thread_getpid());
        
        // Sleep for some time
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    // Initialize message queue
    msg_t msg_queue[8];  // Number of messages must be a power of two
    msg_init_queue(msg_queue, 8);

    // Create a thread for the message sender
    char sender_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(sender_stack, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0, msg_sender, NULL, "msg_sender");

    while (1) {
        msg_t msg;
        // Receive and print message
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return 0;
}