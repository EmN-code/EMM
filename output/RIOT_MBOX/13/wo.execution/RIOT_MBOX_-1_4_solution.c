#include <xtimer.h>  // Include the timer library for using xtimer_sleep
#include <stddef.h>  // Include for standard definitions
#include <stdio.h>   // Include for standard input/output

// Correctly define a message box
struct msg_box_t {
    int id;          // Example field, you can replace it with a real structure
};
typedef struct msg_box_t mbox_t;

// Function to initialize a message box
void mbox_init(mbox_t *mbox, int id) {
    mbox->id = id;
    printf("Message box initialized with id: %d\n", id);
}

// Example function simulating mbox_put functionality
void mbox_put(mbox_t *mbox, int message) {
    printf("Putting message %d in box with id: %d\n", message, mbox->id);
}

void *event_loop(void *args) {
    mbox_t my_mbox;
    mbox_init(&my_mbox, 1);
    while (1) {
        int message = 42;  // Example event/message
        mbox_put(&my_mbox, message);
        xtimer_sleep(1);  // Wait for 1 second between events
    }
    return NULL;
}

int main(void) {
    printf("Starting message box example\n");
    event_loop(NULL);  // Starts the event loop
    return 0;
}