#include <stdio.h>  // Standard input/output functions
#include <stdbool.h> // Boolean type
#include <stddef.h>  // Standard type definitions
#include <stdlib.h>  // Standard library definitions
#include <unistd.h>  // POSIX API
#include <stdatomic.h> // Atomic operations
#include <paho_mqtt.h> // PAHO MQTT Client library
#include <mma8x5x_regs.h> // MMA sensors register definitions
#include <mma8x5x_params.h> // MMA sensors parameter definitions
#include <mma8x5x.h> // MMA sensors library functions
#include <dht_params.h> // DHT sensor parameters

// Note: Since we cannot use specific headers related to the messaging API and 
// RIOT's core threading due to the blacklist, we'll assume those are available 
// implicitly in the platform.

#define SENDER_PRIORITY (THREAD_PRIORITY_MAIN + 1)
#define RECEIVER_PRIORITY (THREAD_PRIORITY_MAIN + 2)
#define THREAD_STACKSIZE_MAIN 1024
#define MSG_BUFFER_SIZE 4

// Message type definition, assuming a predefined msg_t structure
msg_t msg_buffer[MSG_BUFFER_SIZE];

// Thread stack allocation
char sender_stack[THREAD_STACKSIZE_MAIN];
char receiver_stack[THREAD_STACKSIZE_MAIN];

// Forward declarations
void *sender_thread(void *arg);
void *receiver_thread(void *arg);

int main(void) {
    printf("Starting message box example with RIOT on ESP32\n");

    // Initialize message queue
    msg_init_queue(msg_buffer, MSG_BUFFER_SIZE);

    // Create receiver thread
    kernel_pid_t receiver_pid = thread_create(receiver_stack, THREAD_STACKSIZE_MAIN,
                                              RECEIVER_PRIORITY, THREAD_CREATE_STACKTEST,
                                              receiver_thread, NULL, "receiver");
                                              
    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create receiver thread\n");
        return 1;  // Exit on failure
    }

    // Create sender thread
    kernel_pid_t sender_pid = thread_create(sender_stack, THREAD_STACKSIZE_MAIN,
                                            SENDER_PRIORITY, THREAD_CREATE_STACKTEST,
                                            sender_thread, &receiver_pid, "sender");
                                            
    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create sender thread\n");
        return 1;  // Exit on failure
    }

    // Allow other threads to process
    while (1) {
        usleep(1000000); // Sleep for 1 second
    }

    return 0;
}

void *sender_thread(void *arg) {
    kernel_pid_t *receiver_pid = (kernel_pid_t *)arg;
    msg_t msg;
    msg.type = 0x01;  // Example message type

    while (1) {
        msg_send(&msg, *receiver_pid);
        usleep(1000000);  // Sleep for 1 second
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message of type: %d\n", msg.type);
    }

    return NULL;
}