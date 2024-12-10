#include <ztimer.h>
#include <msg.h>
#include <stdio.h>

#define MBOX_SIZE 8
#define INTERVAL (2U * MS_PER_SEC)

static msg_t mbox_buffer[MBOX_SIZE];
static mbox_t mbox;

void sender_task(void) {
    msg_t msg;
    int count = 0;
    while (1) {
        msg.content.value = count++;
        printf("Sending message: %d\n", msg.content.value);
        mbox_put(&mbox, &msg);
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }
}

int main(void) {
    printf("Starting mbox example on ESP32\n");

    // Initialize mbox
    mbox_init(&mbox, mbox_buffer, MBOX_SIZE);

    // Start the sender task
    sender_task();

    // Infinite loop for receiving messages
    msg_t msg;
    while (1) {
        if (mbox_get(&mbox, &msg, 0)) {
            printf("Received message: %d\n", msg.content.value);
        }
    }

    return 0;
}