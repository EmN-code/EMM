
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <unistd.h>
#include <stddef.h>
#include <stdatomic.h>

// Include necessary headers for MQTT and MMA8X5X sensor
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "MQTTClient.h"

// Define the stack size and priority for the threads
#define STACK_SIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

// Define the message queue size
#define MSG_QUEUE_SIZE 16

// Define the message structure
typedef struct {
    kernel_pid_t sender_pid;
    uint16_t type;
    union {
        void *ptr;
        uint32_t value;
    } content;
} msg_t;

// Function prototypes
void sender_thread(void *arg);
void receiver_thread(void *arg);

// Main function
int main(void)
{
    // Initialize the message queue for the main thread
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create the sender thread with high priority
    char sender_stack[STACK_SIZE];
    thread_create(sender_stack, STACK_SIZE, PRIORITY_HIGH, 0, sender_thread, NULL, "sender");

    // Create the receiver thread with low priority
    char receiver_stack[STACK_SIZE];
    thread_create(receiver_stack, STACK_SIZE, PRIORITY_LOW, 0, receiver_thread, NULL, "receiver");

    // Main thread can perform other tasks or simply wait
    while (1) {
        // Main thread can perform other tasks here
        sleep(1);
    }

    return 0;
}

// Sender thread function
void sender_thread(void *arg)
{
    (void)arg;

    msg_t msg;
    msg.type = 1;
    msg.content.value = 1234;

    while (1) {
        // Send a message to the receiver thread
        msg_send_receive(&msg, &msg, thread_getpid());
        sleep(2);
    }
}

// Receiver thread function
void receiver_thread(void *arg)
{
    (void)arg;

    msg_t msg;

    while (1) {
        // Receive a message from the sender thread
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }
}
