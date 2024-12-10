#include "net/sock/util.h"
#include "stdatomic.h"
#include "MQTTClient.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "stdbool.h"
#include "ztimer.h"
#include "msg.h"
#include "dht_params.h"
#include "timex.h"
#include "stddef.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "sys/socket.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "mutex.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "xtimer.h"
#include "mma8x5x_params.h"
#include "thread.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "event/thread.h"

#define THREAD_STACKSIZE_MAIN    (1024)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

static char thread_stack[THREAD_STACKSIZE_MAIN];

void *thread_handler(void *arg) {
    msg_t msg, reply_msg;
    kernel_pid_t sender_pid;
    
    while (1) {
        msg_receive(&msg);
        printf("Received message from thread PID: %d\n", msg.sender_pid);
        
        // Process the message and prepare a reply
        reply_msg.content.value = msg.content.value + 1; // Example processing

        // Send reply
        if (msg_reply(&msg, &reply_msg) < 0) {
            printf("Error sending reply\n");
        }
    }
    return NULL;
}

void sender_task(void) {
    msg_t msg;
    kernel_pid_t receiver_pid = 2;  // Assuming 2 is the PID of the receiver thread
    
    msg.content.value = 42;  // Example content

    // Send the message
    while (msg_send(&msg, receiver_pid) <= 0) {
        printf("Sending message failed; retrying\n");
        usleep(1000000);  // Retry after 1 second
    }
    printf("Message sent successfully\n");
}

int main(void) {
    puts("RIOT Message Passing Example");
    
    kernel_pid_t thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                                             THREAD_PRIORITY_MAIN, 0,
                                             thread_handler, NULL, "receiver");
    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating thread\n");
        return 1;
    }
    
    // Start the sender task
    sender_task();

    return 0;
}