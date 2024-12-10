
#include "esp_system.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define TAG "MBOX_Example"
#define QUEUE_SIZE 8

typedef struct {
    QueueHandle_t queue;
} mbox_t;

typedef struct {
    void *data;
} msg_t;

void mbox_init(mbox_t *mbox, msg_t *queue, unsigned int queue_size) {
    mbox->queue = xQueueCreate(queue_size, sizeof(msg_t));
    if (mbox->queue == NULL) {
        ESP_LOGE(TAG, "Failed to create queue");
    }
}

int mbox_get(mbox_t *mbox, msg_t *msg, int blocking) {
    if (blocking) {
        return xQueueReceive(mbox->queue, msg, portMAX_DELAY) == pdTRUE;
    } else {
        return xQueueReceive(mbox->queue, msg, 0) == pdTRUE;
    }
}

int mbox_put(mbox_t *mbox, msg_t *msg) {
    return xQueueSend(mbox->queue, msg, portMAX_DELAY) == pdTRUE;
}

void sender_task(void *arg) {
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;
    uint32_t count = 0;

    while (1) {
        msg.data = (void *)count;
        if (mbox_put(mbox, &msg)) {
            ESP_LOGI(TAG, "Sent message with count: %lu", count);
        } else {
            ESP_LOGE(TAG, "Failed to send message");
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // Send every 1 second
        count++;
    }
}

void receiver_task(void *arg) {
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;

    while (1) {
        if (mbox_get(mbox, &msg, 1)) {
            ESP_LOGI(TAG, "Received message with count: %lu", (uint32_t)msg.data);
        } else {
            ESP_LOGE(TAG, "Failed to receive message");
        }
    }
}

int main() {
    mbox_t mbox;
    msg_t queue[QUEUE_SIZE];

    mbox_init(&mbox, queue, QUEUE_SIZE);

    xTaskCreate(sender_task, "sender_task", 2048, &mbox, 5, NULL);
    xTaskCreate(receiver_task, "receiver_task", 2048, &mbox, 5, NULL);

    vTaskStartScheduler();

    return 0;
}
