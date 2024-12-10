#include <xtimer.h>
#include <msg.h>
#include <stdio.h>
#include <stdlib.h>

#define MAIN_QUEUE_SIZE (8)
#define MSG_SEND_INTERVAL (1000000U)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

void *event_sender(void *arg) {
    (void)arg;
    msg_t msg;
    int counter = 0;
    while (1) {
        xtimer_sleep_ms(MSG_SEND_INTERVAL);
        msg.type = 1;
        msg.content.value = ++counter;
        if (msg_send(&msg, main_msg_queue) < 0) {
            puts("[Sender] Error sending message.");
        }
        else {
            printf("[Sender] Message sent: %d\n", counter);
        }
    }
    return NULL;
}

void *event_receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        if (msg.type == 1) {
            printf("[Receiver] Received message: %d\n", (int)msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    puts("Message Box Example on RIOT OS with ESP32\n");

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  event_sender, NULL, "event_sender");
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                  event_receiver, NULL, "event_receiver");

    return 0;
}