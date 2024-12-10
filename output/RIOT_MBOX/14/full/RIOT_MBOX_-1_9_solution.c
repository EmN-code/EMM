#include <string.h>  // Required for string operations
#include <sys/socket.h>  // Required for socket operations
#include <stdatomic.h>  // Required for atomic operations
#include <stdbool.h>  // Required for boolean data type
#include <stddef.h>  // Required for standard definitions
#include <stdlib.h>  // Required for general utilities
#include <stdio.h>  // Required for standard I/O operations
#include <unistd.h>  // Required for miscellaneous symbolic constants and types
#include <stdint.h>  // Required for precise integer types
#include <netinet/in.h>  // Required for Internet address family

#include "net/sock/util.h"  // Required for network socket utilities
#include "esp_wifi.h"  // Required for Wi-Fi functionalities
#include "net/sock/udp.h"  // Required for UDP socket
#include "kernel_defines.h"  // Required for kernel definitions
#include "ztimer.h"  // Required for timer functionality

#define MESSAGE_QUEUE_SIZE 4  // Define the size of the message queue

mbox_t my_mbox;  // Define a mailbox
msg_t my_mbox_queue[MESSAGE_QUEUE_SIZE];  // Define a message queue

void *sender_thread(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    msg_t msg;
    msg.content.value = 42;  // Define a message with content
    while (1) {
        msg_send(&msg, thread_getpid());  // Send message
        printf("Sent message with value: %u\n", msg.content.value);
        ztimer_sleep(ZTIMER_SECOND);  // Sleep for a second
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);  // Receive a message
        printf("Received message with value: %u\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(my_mbox_queue, MESSAGE_QUEUE_SIZE);  // Initialize the message queue
    thread_create(NULL, 1024, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");  // Create the sender thread
    thread_create(NULL, 1024, THREAD_PRIORITY_MAIN - 2, 0, receiver_thread, NULL, "receiver");  // Create the receiver thread
    return 0;
}