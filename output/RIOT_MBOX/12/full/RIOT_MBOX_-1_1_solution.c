#include <ztimer.h>
#include <stdio.h>
#include <msg.h>

// Define stack size and the message queue length
#define MAIN_QUEUE_SIZE (8)

// Create a message queue
static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

// Task to periodically send messages
void *msg_sender(void *arg) {
    (void)arg;
    msg_t msg;
    ztimer_t timer;
    msg.content.value = 42;  // Example message content value

    // Initialize the timer to send a message every 1 second
    ztimer_set(ZTIMER_MSEC, &timer, 1000);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        msg.content.value++;
        msg_send(&msg, msg_queue_get_default());
        printf("Message sent with value: %d\n", msg.content.value);
    }

    return NULL;
}

// Main function
int main(void) {
    puts("Message Box Example on ESP32 with RIOT");

    // Initialize the message queue
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    // Create and start the sender thread
    char sender_thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0,
                  msg_sender, NULL, "msg_sender");

    // Main thread work loop
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Message received with value: %d\n", msg.content.value);
    }

    return 0;
}