
#include "mma8x5x.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "stdio.h"
#include "unistd.h"
#include "stdatomic.h"
#include "stddef.h"
#include "time_units.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "stdlib.h"
#include "event/thread.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "esp_system.h"
#include "ztimer.h"
#include "netinet/in.h"
#include "esp_wifi.h"
#include "xtimer.h"
#include "msg.h"
#include "timex.h"
#include "net/sock/udp.h"
#include "evtimer_mbox.h"
#include "mutex.h"
#include "thread.h"
#include "mbox.h"
#include "sys/socket.h"
#include "mma8x5x_regs.h"
#include "dht.h"
#include "stdbool.h"
#include "evtimer_msg.h"
#include "net/sock/util.h"
#include "arpa/inet.h"
#include "kernel_defines.h"

#define QUEUE_SIZE (16)

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
        // Block until a message is available
        while (mbox->read_pos == mbox->write_pos);
    }
    *msg = mbox->queue[mbox->read_pos];
    mbox->read_pos = (mbox->read_pos + 1) % mbox->queue_size;
    return 1;
}

kernel_pid_t thread_getpid(void);

int main(void)
{
    msg_t msg_queue[QUEUE_SIZE];
    mbox_t mbox;
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    msg_t msg;
    msg.type = 1;
    msg.content.value = 0x12345678;

    while (1) {
        mbox_put(&mbox, &msg);
        printf("Message sent\n");
        xtimer_sleep(1); // Send message every second
    }

    return 0;
}
