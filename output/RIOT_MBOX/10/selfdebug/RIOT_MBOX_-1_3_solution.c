// Required header files in descending alphabetical order
#include <xtimer.h>
#include <stdio.h>
#include <msg.h>
#include <xtimer.h> // For timer related functions
#include <stdio.h>  // For input and output operations
#include <msg.h>    // For message handling

#define MAIN_QUEUE_SIZE (8)
#define INTERVAL_US (1U * US_PER_SEC)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void sender_thread(void) {
    msg_t msg;
    char msg_content[] = "Hello, RIOT!";

    msg.content.value = (uint32_t)msg_content;
    while (1) {
        printf("Sending message with content: %s\n", (char *)msg.content.value);
        msg_send(&msg, thread_getpid());
        xtimer_usleep(INTERVAL_US);
    }
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Start sender thread
    printf("Starting sender thread...\n");
    kernel_pid_t sender_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, sender_thread, NULL, "sender");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with content: %s\n", (char *)msg.content.value);
    }

    return 0;
}