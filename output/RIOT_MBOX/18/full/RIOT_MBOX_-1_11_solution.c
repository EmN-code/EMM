#include <stddef.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <string.h>
#include "esp_system.h"
#include "event/thread.h"
#include "mbox.h"
#include "stdio.h"
#include "msg.h"
#include "timex.h"
#include "time_units.h"
#include "ztimer.h"

#define QUEUE_SIZE 8

typedef struct {
    char *message;
} message_box_entry_t;

static ztimer_t timer;
static msg_t queue[QUEUE_SIZE];
static mbox_t mbox;

static void send_message(void) {
    msg_t msg;
    msg.content.ptr = "Hello from ESP32";
    mbox_send(&mbox, &msg);
    printf("Sent: %s\n", (char *)msg.content.ptr);
}

static void timer_callback(void *arg) {
    (void)arg;
    ztimer_set_msg(&timer, ZTIMER_MSEC, 1000, msg_t{.content.ptr="Timer message"}, thread_getpid());
}

int main(void) {
    mbox_init(&mbox, queue, QUEUE_SIZE);
    ztimer_set_callback(&timer, timer_callback, NULL);
    ztimer_start(&timer);

    while (1) {
        msg_t msg;
        if (mbox_receive(&mbox, &msg) == 1) {
            printf("Received: %s\n", (char *)msg.content.ptr);
        }
    }
    return 0;
}