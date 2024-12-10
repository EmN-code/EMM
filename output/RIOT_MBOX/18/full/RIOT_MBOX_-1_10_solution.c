#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>

#define MESSAGE_QUEUE_SIZE 16

typedef struct {
    int sender_id;
    int message_type;
    union {
        int int_value;
        void *ptr_value;
    } content;
} msg_t;

typedef struct {
    msg_t queue[MESSAGE_QUEUE_SIZE];
    int head;
    int tail;
    SemaphoreHandle_t lock;
    SemaphoreHandle_t message_available;
} message_box_t;

void message_box_init(message_box_t *mbox) {
    mbox->head = 0;
    mbox->tail = 0;
    mbox->lock = xSemaphoreCreateMutex();
    mbox->message_available = xSemaphoreCreateBinary();
}

void message_box_send(message_box_t *mbox, msg_t *msg) {
    xSemaphoreTake(mbox->lock, portMAX_DELAY);
    mbox->queue[mbox->tail] = *msg;
    mbox->tail = (mbox->tail + 1) % MESSAGE_QUEUE_SIZE;
    xSemaphoreGive(mbox->lock);
    xSemaphoreGive(mbox->message_available);
}

void message_box_receive(message_box_t *mbox, msg_t *msg) {
    xSemaphoreTake(mbox->message_available, portMAX_DELAY);
    xSemaphoreTake(mbox->lock, portMAX_DELAY);
    *msg = mbox->queue[mbox->head];
    mbox->head = (mbox->head + 1) % MESSAGE_QUEUE_SIZE;
    xSemaphoreGive(mbox->lock);
}

void sender_task(void *param) {
    message_box_t *mbox = (message_box_t *)param;
    msg_t msg;
    msg.sender_id = 1;
    msg.message_type = 1;  // simple message type
    msg.content.int_value = 123;

    while (1) {
        printf("Sending message with value: %d\n", msg.content.int_value);
        message_box_send(mbox, &msg);
        vTaskDelay(pdMS_TO_TICKS(1000));  // Send every 1 second
    }
}

void receiver_task(void *param) {
    message_box_t *mbox = (message_box_t *)param;
    msg_t msg;

    while (1) {
        message_box_receive(mbox, &msg);
        printf("Received message from sender %d with value: %d\n", msg.sender_id, msg.content.int_value);
    }
}

int main(void) {
    message_box_t mbox;
    message_box_init(&mbox);

    xTaskCreate(sender_task, "SenderTask", 2048, (void *)&mbox, 5, NULL);
    xTaskCreate(receiver_task, "ReceiverTask", 2048, (void *)&mbox, 5, NULL);

    vTaskStartScheduler();

    while (1) {
        // Should never be reached
    }
    return 0;
}