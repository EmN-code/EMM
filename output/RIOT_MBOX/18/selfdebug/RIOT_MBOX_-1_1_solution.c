#include <ztimer.h>
#include <msg.h>
#include <evtimer_msg_event_t.h>

#define INTERVAL (ZTIMER_SEC(1))  // 1-second interval 
#define MESSAGE_TYPE (0x1234)

static msg_t queue[8];

void callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = MESSAGE_TYPE;
    msg_send(&msg, thread_getpid());
    printf("Message sent with type: %x\n", MESSAGE_TYPE);
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    evtimer_t timer;
    ztimer_init_anycast(&timer, ZTIMER_MSEC_ADVANCED);
    evtimer_msg_event_t event = {
        .event.msg = {.type = MESSAGE_TYPE}
    };
    evtimer_set_msg(&timer, &event.event, INTERVAL);

    puts("Starting event timer\n");
    evtimer_start(&timer);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == MESSAGE_TYPE) {
            printf("Received message of type: %x\n", msg.type);
            evtimer_set_msg(&timer, &event.event, INTERVAL);
        }
    }

    return 0;
}