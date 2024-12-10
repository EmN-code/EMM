#include <ztimer.h>
#include <msg.h>
#include <evtimer.h>

#define INTERVAL (1000U * 1000U) // 1 second interval

static msg_t msg_queue[8]; // Message queue

typedef struct {
    evtimer_event_t event;
    msg_t msg;
} evtimer_msg_event_t;

static void msg_sender(void) {
    while (1) {
        msg_t msg;
        msg.content.value = 42; // Example payload
        msg_send(&msg, thread_getpid());
        ztimer_sleep(ZTIMER_USEC, INTERVAL); // Sleep for the interval
    }
}

void *receiver_thread(void *arg) {
    (void)arg;
    evtimer_msg_event_t evt;
    evtimer_t timer;
    evt.timer = &timer;

    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    while (1) {
        msg_receive(&evt.msg);
        printf("Received message with value: %d\n", evt.msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(NULL, THREAD_STACKSIZE_DEFAULT,
                                              THREAD_PRIORITY_MAIN - 1,
                                              THREAD_CREATE_STACKTEST,
                                              receiver_thread, NULL, "receiver");
    msg_sender();

    return 0;
}