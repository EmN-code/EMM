#include <xtimer.h>  // Required for using RIOT's timer
#include <thread.h>   // To manage thread functions
#include <stdio.h>    // For input and output functions
#include <msg.h>      // To work with message queues

#define MAIN_QUEUE_SIZE     (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

char receiver_thread_stack[THREAD_STACKSIZE_DEFAULT];  // Define thread stack

void *_receiver_thread(void *arg) {
    msg_t msg;
    (void)arg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with content: %lu\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    puts("Message queue example on RIOT with ESP32\n");

    // Start receiver thread
    thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  _receiver_thread, NULL, "receiver");

    msg_t msg;
    msg.content.value = 42;  // Just a random number to send

    while (1) {
        msg_send(&msg, thread_getpid());  // Send self message
        xtimer_sleep(5);  // Wait for 5 seconds before sending again
    }

    return 0;
}