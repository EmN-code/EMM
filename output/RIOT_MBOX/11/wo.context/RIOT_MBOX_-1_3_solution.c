#include "xtimer.h"
#include "msg.h"
#include "event/callback.h"
#include "evtimer.h"

#define INTERVAL 1000000 // Interval for events in microseconds

static evtimer_t event_timer;
static msg_t msg_queue[8]; // Message queue for 8 messages

typedef struct {
    uint32_t type;
    char *content;
} my_message_t;

static void my_callback(void *arg) {
    my_message_t *message = (my_message_t *)arg;
    printf("Received message of type: %lu\n", message->type);
    printf("Content: %s\n", message->content);
}

int main(void) {
    // Initialize message queue
    msg_init_queue(msg_queue, 8);
    
    // Initialize event timer
    evtimer_init(&event_timer, my_callback);

    my_message_t message;
    message.type = 42; // Example type
    message.content = "Hello, RIOT!"; // Example content

    evtimer_add(&event_timer.timer, INTERVAL, &message);

    // Run indefinitely
    while (1) {
        msg_t m;
        // Wait and process the next message
        msg_receive(&m);
        printf("Processed an event!\n");
    }
    return 0;
}