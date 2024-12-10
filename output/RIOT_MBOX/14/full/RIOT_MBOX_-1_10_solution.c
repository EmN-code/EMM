#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <timex.h> // Considering this header as its equivalent
#include <ztimer.h> // Assuming the correct alternative
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <msg.h>
#include <thread.h>
#include <mutex.h>
#include <dht_params.h>
#include <dht.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <sys/socket.h>
#include <kernel_defines.h>
#define MAIN_QUEUE_SIZE     (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void sender_thread(void) {
    msg_t msg;
    msg.content.value = 0x12345678; // Example message content

    while (1) {
        // Send message to the receiver thread
        if (msg_send(&msg, thread_getpid()) == 1) {
            printf("Message sent: 0x%08lx\n", (unsigned long)msg.content.value);
        }
        else {
            printf("Failed to send message\n");
        }

        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
    }
}

void receiver_thread(void) {
    msg_t msg;

    while (1) {
        // Receive message
        msg_receive(&msg);
        printf("Message received: 0x%08lx\n", (unsigned long)msg.content.value);
    }
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // create sender and receiver threads
    char sender_stack[THREAD_STACKSIZE_DEFAULT];
    char receiver_stack[THREAD_STACKSIZE_DEFAULT];

    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, (thread_task_func_t)sender_thread,
                  NULL, "sender_thread");

    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, (thread_task_func_t)receiver_thread,
                  NULL, "receiver_thread");

    return 0;
}