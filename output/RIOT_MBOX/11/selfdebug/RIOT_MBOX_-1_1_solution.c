#include <ztimer.h>   // For timer functionalities
#include <msg.h>       // For message box functionalities
#include <stdio.h>     // For standard input-output

#define INTERVAL (2U * ZTIMER_MSEC_PER_SEC)  // Define interval as 2 seconds
#define MAIN_QUEUE_SIZE (8)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static mbox_t mbox;
static char thread_stack[THREAD_STACKSIZE_DEFAULT];

// Thread function to periodically send messages
void *message_thread(void *arg) {
    (void)arg;
    msg_t msg;
    ztimer_periodic_t periodic_timer;

    // Initialize periodic timer
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_timer, INTERVAL);

    while (1) {
        ztimer_periodic_wait(&periodic_timer);
        printf("Sending message...\n");

        msg.content.value = 1;  // Example content
        if (mbox_send(&mbox, &msg) != 1) {
            puts("Message sending failed!");
        }
    }

    return NULL;
}

// Main function
int main(void) {
    puts("RIOT message box example");

    // Initialize message box
    mbox_init(&mbox, main_msg_queue, MAIN_QUEUE_SIZE);

    // Start the thread for message handling
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, 0, message_thread, NULL, "msg_thread");

    // Main loop to receive messages
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %ld\n", msg.content.value);
    }

    // Program should never reach here
    return 0;
}