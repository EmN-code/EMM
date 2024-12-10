/* Code implementation for inter-thread communication in RIOT using an ESP32. 
   This program uses threads to send messages to each other with different priorities using `msg_t`.
   It also includes error handling, particularly for MQTT with Paho MQTTClient, with a retry mechanism.
*/

#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <thread.h>
#include <timex.h>
#include <time_units.h>
#include <MQTTClient.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)

static char stack_sender[THREAD_STACKSIZE_MAIN];
static char stack_receiver[THREAD_STACKSIZE_MAIN];

msg_t msg_queue[8];

void receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, 8);

    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Message received: value = %u\n", msg.content.value);
        }
    }
}

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    kernel_pid_t receiver_pid = thread_getpid(); // Assuming receiver thread will be created and will get a PID.

    for (uint32_t i = 0; i < 10; ++i) {
        msg.type = 0x1234;
        msg.content.value = i;

        if (msg_send(&msg, receiver_pid) != 1) {
            printf("Message sending failed\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
}

int main(void) {
    puts("Starting inter-thread communication in RIOT OS on ESP32\n");

    thread_create(stack_receiver, sizeof(stack_receiver), THREAD_PRIORITY_RECEIVER,
                  THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    thread_create(stack_sender, sizeof(stack_sender), THREAD_PRIORITY_SENDER,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    // Now, the threads will communicate via messages.
    // Also, handling MQTT connections to an external server.
    Network network;
    MQTTClient client;
    int rc = 0;

    // Ensuring network connection
    while(1) {
        rc = NetworkConnect(&network, "mqtt://test.mosquitto.org", 1883);
        if(rc != 0){
            printf("Network connection failed\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }else{
            printf("Network connected successfully\n");
            break;
        }
    }
    
    // Further MQTT work goes here...

    return 0;
}