
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <esp_system.h>
#include <net/sock/udp.h>
#include <dht.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <kernel_defines.h>

#define QUEUE_SIZE 8

typedef struct {
    kernel_pid_t sender_pid;
    uint16_t type;
    union {
        void *ptr;
        uint32_t value;
    } content;
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
        // Blocking behavior not implemented here for simplicity
    }
    *msg = mbox->queue[mbox->read_pos];
    mbox->read_pos = (mbox->read_pos + 1) % mbox->queue_size;
    return 1;
}

static void *sender_thread(void *arg)
{
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        mbox_put(mbox, &msg);
        printf("Sent message with value: %lu\n", msg.content.value);
        sleep(1); // Sleep for 1 second
    }

    return NULL;
}

static void *receiver_thread(void *arg)
{
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;

    while (1) {
        if (mbox_get(mbox, &msg, 1)) {
            printf("Received message with value: %lu\n", msg.content.value);
        }
    }

    return NULL;
}

int main(void)
{
    msg_t queue[QUEUE_SIZE];
    mbox_t mbox;
    mbox_init(&mbox, queue, QUEUE_SIZE);

    pthread_t sender, receiver;
    pthread_create(&sender, NULL, sender_thread, &mbox);
    pthread_create(&receiver, NULL, receiver_thread, &mbox);

    pthread_join(sender, NULL);
    pthread_join(receiver, NULL);

    return 0;
}
