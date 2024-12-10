
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <mbox.h>
#include <timex.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <xtimer.h>
#include <ztimer.h>

#define STACK_SIZE 1024
#define MSG_QUEUE_SIZE 8

typedef struct {
    uint16_t type;
    union {
        void *ptr;
        uint32_t value;
    } content;
} msg_t;

static msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void *arg) {
    msg_t msg;
    msg.type = 1;
    msg.content.value = 1234;

    while (1) {
        printf("Sender: Sending message...\n");
        if (msg_send(&msg, (kernel_pid_t)arg) == 1) {
            printf("Sender: Message sent successfully.\n");
        } else {
            printf("Sender: Failed to send message.\n");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void receiver_thread(void *arg) {
    msg_t msg;

    while (1) {
        printf("Receiver: Waiting for message...\n");
        if (msg_receive(&msg) == 1) {
            printf("Receiver: Message received with type %u and value %u.\n", msg.type, msg.content.value);
        } else {
            printf("Receiver: Failed to receive message.\n");
        }
    }
}

int main() {
    kernel_pid_t sender_pid, receiver_pid;
    char sender_stack[STACK_SIZE];
    char receiver_stack[STACK_SIZE];

    printf("Initializing message queue...\n");
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    printf("Creating sender thread...\n");
    sender_pid = thread_create(sender_stack, STACK_SIZE, 1, 0, (thread_task_func_t)sender_thread, (void *)receiver_pid, "sender");

    printf("Creating receiver thread...\n");
    receiver_pid = thread_create(receiver_stack, STACK_SIZE, 2, 0, (thread_task_func_t)receiver_thread, NULL, "receiver");

    printf("Main thread: Starting scheduler...\n");
    vTaskStartScheduler();

    return 0;
}
