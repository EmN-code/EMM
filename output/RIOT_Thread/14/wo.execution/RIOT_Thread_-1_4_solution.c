#include <xtimer.h>
#include <ztimer.h>
#include <time_units.h>
#include <timex.h>
#include <thread.h>
#include <msg.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_timer.h>  // Needed for esp_random
#include <esp_system.h>  // Needed for esp_random
#include <pthread.h>  // Needed for thread_get_id

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)
#define SENDER_STACKSIZE THREAD_STACKSIZE_DEFAULT
#define RECEIVER_STACKSIZE THREAD_STACKSIZE_DEFAULT

char sender_stack[SENDER_STACKSIZE];
char receiver_stack[RECEIVER_STACKSIZE];

static kernel_pid_t sender_pid;
static kernel_pid_t receiver_pid;

void *receiver(void *arg) {
    msg_t msg;
    (void)arg;
    
    while (1) {
        msg_receive(&msg);
        printf("Received message of type: %d from PID: %d\n", msg.type, msg.sender_pid);
    }

    return NULL;
}

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = (uint16_t) esp_random();

    while (1) {
        xtimer_sleep(1);
        printf("Sending message of type: %u\n", msg.type);
        msg_send(&msg, receiver_pid);
        msg.type = (uint16_t) esp_random();
    }

    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, RECEIVER_STACKSIZE, RECEIVER_PRIO, 0, receiver, NULL, "receiver");
    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating receiver thread\n");
        return 1;
    }

    sender_pid = thread_create(sender_stack, SENDER_STACKSIZE, SENDER_PRIO, 0, sender, NULL, "sender");
    if (sender_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating sender thread\n");
        return 1;
    }

    return 0;
}
