#include <netinet/in.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <string.h>
#include <xtimer.h>
#include <time_units.h>
#include <timex.h>
#include <ztimer.h>
#include <event/thread.h>
#include <kernel_defines.h>
#include <msg.h>
#include <unistd.h>
#include <pthread.h>
#include <esp_system.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <MBQTTClient.h>
#include <paho_mqtt.h>
#include <mutex.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <mma8x5x_regs.h>

#define THREAD_STACKSIZE_MAIN 1024  // Define stack size
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)  // Define thread priority

// Global message queue
static msg_t msg_queue[8];

// Task function for sending messages
void *sender_task(void *arg) {
    (void) arg;
    msg_t msg;
    msg.type = 1;
    while (1) {
        printf("Sending message...\n");
        msg_send(&msg, thread_getpid());
        xtimer_sleep(4);  // Sleep for 4 seconds
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_t));  // Initialize message queue

    // Creation of sender thread
    char sender_stack[THREAD_STACKSIZE_MAIN];
    kernel_pid_t sender_pid = thread_create(
        sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN,
        CREATE_STACKTEST, sender_task, NULL, "sender_thread"
    );

    if (sender_pid <= 0) {
        printf("Failed to create sender_thread\n");
    }
    else {
        printf("Sender thread PID: %d\n", sender_pid);
    }

    msg_t msg;
    while (1) {
        if (msg_receive(&msg) > 0) {
            printf("Message received: type = %u\n", (unsigned int)msg.type);
        }
    }
    return 0;
}