#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

static kernel_pid_t receiver_pid;

static void *receiver_thread(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", msg.content.value);
    }
    return NULL;
}

static void *sender_thread(void *arg) {
    msg_t msg;
    msg.type = 0x1234;

    // Improved network connection with retry logic
    int rc;
    while (1) {
        // Simulating a network connection function
        rc = rand() % 2; // Assume 0 means success, 1 means failure
        if(rc != 0){
            printf("Connection failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connection successful\n");
            break;
        }
    }

    while (1) {
        msg.content.value = rand();
        msg_send(&msg, receiver_pid);
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_RECEIVER, 0,
                                 receiver_thread, NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack),
                  THREAD_PRIORITY_SENDER, 0,
                  sender_thread, NULL, "sender");

    return 0;
}