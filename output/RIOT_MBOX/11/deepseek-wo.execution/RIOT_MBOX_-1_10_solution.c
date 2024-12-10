
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <sys/socket.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <esp_system.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#define QUEUE_SIZE 8

typedef struct {
    void *content;
} msg_t;

typedef struct {
    msg_t *queue;
    unsigned int queue_size;
    unsigned int read_pos;
    unsigned int write_pos;
} mbox_t;

static inline void mbox_init(mbox_t *mbox, msg_t *queue, unsigned int queue_size)
{
    mbox->queue = queue;
    mbox->queue_size = queue_size;
    mbox->read_pos = 0;
    mbox->write_pos = 0;
}

static inline int mbox_put(mbox_t *mbox, msg_t *msg)
{
    unsigned int next_write_pos = (mbox->write_pos + 1) % mbox->queue_size;
    if (next_write_pos == mbox->read_pos) {
        return 0; // Queue is full
    }
    mbox->queue[mbox->write_pos] = *msg;
    mbox->write_pos = next_write_pos;
    return 1;
}

static inline int mbox_get(mbox_t *mbox, msg_t *msg, int blocking)
{
    if (mbox->read_pos == mbox->write_pos) {
        if (!blocking) {
            return 0; // Queue is empty
        }
        // Blocking implementation would go here
    }
    *msg = mbox->queue[mbox->read_pos];
    mbox->read_pos = (mbox->read_pos + 1) % mbox->queue_size;
    return 1;
}

void sender_task(void *arg)
{
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;
    while (1) {
        msg.content = (void *)"Event";
        mbox_put(mbox, &msg);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Send event every 1 second
    }
}

void receiver_task(void *arg)
{
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;
    while (1) {
        if (mbox_get(mbox, &msg, 1)) {
            printf("Received: %s\n", (char *)msg.content);
        }
    }
}

int main()
{
    msg_t queue[QUEUE_SIZE];
    mbox_t mbox;
    mbox_init(&mbox, queue, QUEUE_SIZE);

    xTaskCreate(sender_task, "Sender", 2048, &mbox, 1, NULL);
    xTaskCreate(receiver_task, "Receiver", 2048, &mbox, 1, NULL);

    vTaskStartScheduler();

    return 0;
}
