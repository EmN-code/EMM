#include <ztimer.h> 
#include <msg.h> 
#include <stdio.h> 

#define TIMER_INTERVAL 1000000 // 1 second interval
#define MSG_TYPE 0x1234

static ztimer_t timer;
static msg_t msg_queue[8]; // Message queue with 8 slots
static kernel_pid_t pid;

// Timer callback function
static void timer_callback(void *arg) {
    msg_t msg;
    msg.type = MSG_TYPE;
    msg.content.value = (int)arg;
    printf("Sending message with value: %d\n", msg.content.value);
    msg_send(&msg, pid);
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_t));
    
    pid = thread_getpid();

    // Initialize the timer
    ztimer_set_msg(ZTIMER_MSEC, &timer, TIMER_INTERVAL, &msg_queue[0], pid);

    // Set the timer's callback function
    timer.callback = timer_callback;

    printf("Message box initialized. Sending messages every %d ms.\n", TIMER_INTERVAL);

    while (1) {
        // Receiving messages
        msg_t m;
        msg_receive(&m);
        if (m.type == MSG_TYPE) {
            printf("Received message with value: %d\n", m.content.value);
        }
    }

    return 0;
}
