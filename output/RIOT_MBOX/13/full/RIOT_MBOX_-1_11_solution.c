// Required Imports with alphabetical order
#include <ztimer.h> // For timers
#include <xtimer.h> // For time management
#include <stdio.h> // For standard I/O
#include <stdlib.h> // For standard library functions
#include <stdint.h> // For standard int types
#include <stdbool.h> // For boolean type
#include <msg.h> // For message queue
#include <kernel_types.h> // For kernel types like kernel_pid_t
#include <thread.h> // For thread functions
#include <net/mqttsn.h> // For MQTT-SN related functions

#define SENDER_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define MESSAGE_QUEUE_SIZE (8)
#define MQTT_BROKER_IP "192.168.1.10"
#define MQTT_BROKER_PORT 1883

static char sender_stack[SENDER_STACK_SIZE];
static char receiver_stack[RECEIVER_STACK_SIZE];
static msg_t msg_queue[MESSAGE_QUEUE_SIZE];

void *receiver(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, MESSAGE_QUEUE_SIZE);
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message of type %u\n", msg.type);
    }
    return NULL;
}

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg.type = 0x1234; // Example message type
        msg.content.value = 42; // Example content

        // Send message to the receiver thread
        if (msg_send(&msg, thread_getpid_of(receiver_stack)) <= 0) {
            printf("Failed to send message\n");
        }

        // Introduce delay between messages
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    printf("Starting message box functionality\n");

    if (thread_create(receiver_stack, RECEIVER_STACK_SIZE, 1,
                      THREAD_CREATE_STACKTEST, receiver, NULL,
                      "receiver") == KERNEL_PID_UNDEF) {
        printf("Failed to create receiver thread\n");
        return 1;
    }

    if (thread_create(sender_stack, SENDER_STACK_SIZE, 1,
                      THREAD_CREATE_STACKTEST, sender, NULL,
                      "sender") == KERNEL_PID_UNDEF) {
        printf("Failed to create sender thread\n");
        return 1;
    }

    // Main thread can do other tasks or yield
    while (1) {
        thread_yield();
    }

    return 0;
}