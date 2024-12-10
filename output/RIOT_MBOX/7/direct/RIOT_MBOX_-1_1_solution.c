#include <msg.h>  
#include <xtimer.h>
#include <stdio.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN < 128 ? 128 : THREAD_STACKSIZE_MAIN)
#define MAIN_QUEUE_SIZE 8

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *message_sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    int counter = 0;

    while (1) {
        xtimer_sleep(2);  // Sleep for 2 seconds
        msg.content.value = counter++;
        msg_send(&msg, sched_active_thread);  // Send message to the main thread
        printf("Sent message with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    printf("Message Box Example on ESP32 with RIOT\n");

    char sender_thread_stack[THREAD_STACKSIZE];
    thread_create(sender_thread_stack, sizeof(sender_thread_stack), 
                  THREAD_PRIORITY_MAIN - 1, 0, 
                  message_sender_thread, NULL, "sender");

    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return 0;
}