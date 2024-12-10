#include "xtimer.h"
#include "stdio.h"

#define MSG_QUEUE_SIZE 8

typedef struct {
    int message_type;
    char message_content[50];
} message_t;

static msg_t msg_queue[MSG_QUEUE_SIZE];
static message_t messages[MSG_QUEUE_SIZE];

void *sender(void *arg) {
    (void)arg;

    while (1) {
        msg_t msg;
        message_t message;

        msg.type = 1;
        snprintf(message.message_content, sizeof(message.message_content), "Hello from sender!");
        messages[0] = message;
        msg.content.ptr = &messages[0];

        msg_send(&msg, thread_getpid());

        printf("Message sent: %s\n", message.message_content);

        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN - 1, 0, sender, NULL, "sender_thread");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        message_t *received_message = (message_t *)msg.content.ptr;

        if (received_message) {
            printf("Received message: %s\n", received_message->message_content);
        }
    }

    return 0;
}