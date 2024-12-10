#include <xtimer.h>
#include <unistd.h>
#include <ztimer.h>
#include <timex.h>
#include <thread.h>
#include <msg.h>
#include <mbox.h>
#include <event/thread.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <time_units.h>

// Replace placeholder values with actual parameters
#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define CONFIG_MAIN_QUEUE_SIZE 8

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

msg_t main_msg_queue[CONFIG_MAIN_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    uint32_t counter = 0;

    while (1) {
        msg.content.value = counter++;
        msg_send(&msg, thread_getpid_of((thread_t *)arg));
        xtimer_sleep(1);
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %ld\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, CONFIG_MAIN_QUEUE_SIZE);

    kernel_pid_t receiver_pid = thread_create(
        receiver_stack, THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_MAIN, 0,
        receiver_thread, NULL, "receiver"
    );

    thread_create(
        sender_stack, THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_MAIN, 0,
        sender_thread, (void *)receiver_pid, "sender"
    );

    return 0;
}