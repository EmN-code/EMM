#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mbox.h>
#include <MQTTClient.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <time_units.h>
#include <event/thread.h>
#include <thread.h>
#include <kernel_defines.h>

#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE_DEFAULT (1024)
#define MAIN_QUEUE_SIZE         (8)

enum {
    MQTT_RECONNECT_MAX_RETRIES = 5
};

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static char stack[THREAD_STACKSIZE_DEFAULT];
static char stack2[THREAD_STACKSIZE_DEFAULT];

static void *receiver_thread(void *arg) {
    (void) arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message type: %u from sender: %d\n", msg.type, msg.sender_pid);
    }
    return NULL;
}

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Arbitrary data for example
    msg.type = 0x55;        // Arbitrary type
    kernel_pid_t receiver_pid = (kernel_pid_t)arg;
    
    while (1) {
        printf("Sending message\n");
        msg_send(&msg, receiver_pid);
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    
    kernel_pid_t receiver_pid = thread_create(stack2, sizeof(stack2), RECEIVER_THREAD_PRIORITY, 0, receiver_thread, NULL, "receiver");
    
    while(receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create receiver thread, retrying...\n");
        receiver_pid = thread_create(stack2, sizeof(stack2), RECEIVER_THREAD_PRIORITY, 0, receiver_thread, NULL, "receiver");
    }

    thread_create(stack, sizeof(stack), SENDER_THREAD_PRIORITY, 0, sender_thread, (void*)receiver_pid, "sender");
    
    Network network;
    int rc;
    int retry_count = 0;
    while (retry_count++ < MQTT_RECONNECT_MAX_RETRIES) {
        rc = NetworkConnect(&network, "MQTT_BROKER_IP", MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connect attempt %d failed, retrying...\n", retry_count);
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected to network successfully\n");
            break;
        }
    }
    
    if (rc != 0) {
        printf("Failed to connect to MQTT broker after %d attempts\n", MQTT_RECONNECT_MAX_RETRIES);
        return 1;
    }
   
    return 0;
}