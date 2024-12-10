#include "xtimer.h"
#include "stdio.h"
#include "netinet/in.h"
#include "stddef.h"
#include "stdbool.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "string.h"
#include "arpa/inet.h"
#include "esp_system.h"

#define MQ_LEN      8
#define EVENT_INTERVAL   (1000000U)  // 1 second
#define MSG_TYPE_EVENT   (0x1234)

// Substitute for evtimer_mbox_event_t and related functionalities

typedef struct {
    int type;
    char content[64];
} my_event_t;

msg_t msg_queue[MQ_LEN];
mbox_t mbox;

void event_sender(void) {
    my_event_t event;
    msg_t msg;

    while (1) {
        strcpy(event.content, "Hello world");
        event.type = MSG_TYPE_EVENT;

        msg.content.ptr = &event;

        if (mbox_try_send(&mbox, &msg) <= 0) {
            printf("[Error] Could not send message\n");
        }
        xtimer_usleep(EVENT_INTERVAL);
    }
}

void event_receiver(void) {
    msg_t msg;
    
    while (1) {
        if (mbox_receive(&mbox, &msg)) {
            my_event_t *event = (my_event_t *)msg.content.ptr;
            if (event->type == MSG_TYPE_EVENT) {
                printf("Received Event: %s\n", event->content);
            }
        }
    }
}

int main(void) {
    mbox_init(&mbox, msg_queue, MQ_LEN);
    printf("Starting event handling\n");

    // creating threads for sender and receiver
    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, event_sender, NULL, "event_sender");

    thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                  THREAD_PRIORITY_MAIN - 2, 0, event_receiver, NULL, "event_receiver");

    return 0;
}
