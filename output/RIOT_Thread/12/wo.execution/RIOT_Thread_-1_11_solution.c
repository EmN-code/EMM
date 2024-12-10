#include "ztimer.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "sys/socket.h"
#include "stdio.h"
#include "string.h" // For memset function

#define THREAD_PRIORITY_MAIN 5
#define THREAD_PRIORITY_SENDER 6
#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_STACKSIZE_SENDER 1024

// Declare the message structure
msg_t queue[8];  // Define a queue that can hold 8 messages

void sender(void) {
    msg_t msg;
    msg.content.value = 42; // Dummy message content
    msg_send(&msg, thread_getpid()); // Send a message to itself for demo
}

int main(void) {
    msg_init_queue(queue, 8);

    char sender_stack[THREAD_STACKSIZE_SENDER];
    memset(sender_stack, 0, THREAD_STACKSIZE_SENDER);
    
    thread_create(sender_stack, THREAD_STACKSIZE_SENDER, THREAD_PRIORITY_SENDER, 0, (thread_task_func_t)sender, NULL, "sender");
    
    return 0;
}