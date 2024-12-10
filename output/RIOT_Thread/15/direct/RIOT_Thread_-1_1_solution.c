#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread.h>
#include <msg.h>

#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW  (THREAD_PRIORITY_MAIN + 1)
#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_MAIN)

void *high_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;
    int msg_content = 42; // example message content

    msg.content.value = msg_content;
    printf("High priority thread: Sending message with content: %d\n", msg_content);
    if (msg_send(&msg, thread_getpid()) != 1) {
        puts("High priority thread: Failed to send message.");
    } else {
        puts("High priority thread: Message sent successfully.");
    }
    return NULL;
}

void *low_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;

    puts("Low priority thread: Waiting to receive message...");
    if (msg_receive(&msg) == 1) {
        printf("Low priority thread: Received message with content: %d\n", msg.content.value);
    } else {
        puts("Low priority thread: Failed to receive message.");
    }
    return NULL;
}

int main(void) {
    msg_init_queue(NULL, 8); // Initialize a message queue with 8 messages

    char high_thread_stack[THREAD_STACKSIZE_DEFAULT];
    char low_thread_stack[THREAD_STACKSIZE_DEFAULT];

    thread_create(
        high_thread_stack, sizeof(high_thread_stack),
        THREAD_PRIORITY_HIGH, 0,
        high_priority_thread, NULL, "high_priority_thread"
    );

    thread_create(
        low_thread_stack, sizeof(low_thread_stack),
        THREAD_PRIORITY_LOW, 0,
        low_priority_thread, NULL, "low_priority_thread"
    );

    return 0;
}