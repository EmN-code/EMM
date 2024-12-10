
#include <stdio.h>
#include <stdlib.h>
#include <timex.h>
#include <freertos/task.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <sys/types.h>
#include <esp_system.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <sys/socket.h>

#define STACK_SIZE 1024
#define MSG_QUEUE_SIZE 16

typedef struct {
    kernel_pid_t sender_pid;
    uint16_t type;
    union {
        void *ptr;
        uint32_t value;
    } content;
} msg_t;

msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void *arg) {
    msg_t msg;
    msg.type = 1;
    msg.content.value = 1234;

    while (1) {
        msg_send_receive(&msg, &msg, (kernel_pid_t)arg);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }
}

void receiver_thread(void *arg) {
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %u\n", msg.content.value);
        msg_reply(&msg, &msg);
    }
}

int main() {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(
        malloc(STACK_SIZE),
        STACK_SIZE,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender"
    );

    kernel_pid_t receiver_pid = thread_create(
        malloc(STACK_SIZE),
        STACK_SIZE,
        THREAD_PRIORITY_MAIN - 2,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver"
    );

    sender_thread((void *)receiver_pid);

    return 0;
}
