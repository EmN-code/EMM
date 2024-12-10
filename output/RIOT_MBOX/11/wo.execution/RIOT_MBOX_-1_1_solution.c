#include <ztimer.h>
#include <stdio.h>
#include <msg.h>

#define INTERVAL (1U * ZTIMER_SEC) // Timer interval of 1 second
#define QUEUE_SIZE (8U)

static ztimer_t timer;
static msg_t queue[QUEUE_SIZE];
static msg_t msg;
static kernel_pid_t own_pid;

void timer_callback(void *arg) {
    (void)arg;
    printf("Sending message from timer callback\n");
    msg_t msg;
    msg_send(&msg, own_pid); // Send message to itself
    ztimer_set(&timer, INTERVAL); // Restart timer
}

int main(void) {
    own_pid = thread_getpid();
    msg_init_queue(queue, QUEUE_SIZE);

    printf("Starting message box example on ESP32\n");

    // Initialize the ztimer and set a callback
    ztimer_set(&timer, INTERVAL);
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer initially
    ztimer_set(&timer, INTERVAL);

    while (1) {
        msg_receive(&msg); // Wait to receive a message
        printf("Message received\n");
    }
    return 0;
}