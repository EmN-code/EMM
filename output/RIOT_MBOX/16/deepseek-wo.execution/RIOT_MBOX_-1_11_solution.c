
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <time_units.h>
#include <stdlib.h>
#include <event/thread.h>
#include <esp_system.h>
#include <ztimer.h>
#include <netinet/in.h>
#include <esp_wifi.h>
#include <xtimer.h>
#include <msg.h>
#include <timex.h>
#include <net/sock/udp.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <thread.h>
#include <mbox.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <evtimer_msg.h>
#include <net/sock/util.h>
#include <arpa/inet.h>
#include <kernel_defines.h>

#define QUEUE_SIZE 8

typedef struct {
    msg_t *queue;
    unsigned int queue_size;
    atomic_int read_pos;
    atomic_int write_pos;
} mbox_t;

void mbox_init(mbox_t *mbox, msg_t *queue, unsigned int queue_size) {
    mbox->queue = queue;
    mbox->queue_size = queue_size;
    atomic_store(&mbox->read_pos, 0);
    atomic_store(&mbox->write_pos, 0);
}

int _mbox_get(mbox_t *mbox, msg_t *msg, int blocking) {
    int read_pos = atomic_load(&mbox->read_pos);
    int write_pos = atomic_load(&mbox->write_pos);

    if (read_pos == write_pos) {
        if (!blocking) {
            return 0;
        }
        while (read_pos == write_pos) {
            // Busy wait or use a more efficient wait mechanism if available
        }
    }

    *msg = mbox->queue[read_pos];
    read_pos = (read_pos + 1) % mbox->queue_size;
    atomic_store(&mbox->read_pos, read_pos);
    return 1;
}

void sender_thread(void *arg) {
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        int write_pos = atomic_load(&mbox->write_pos);
        mbox->queue[write_pos] = msg;
        write_pos = (write_pos + 1) % mbox->queue_size;
        atomic_store(&mbox->write_pos, write_pos);

        xtimer_sleep(1); // Send event every 1 second
    }
}

void receiver_thread(void *arg) {
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;

    while (1) {
        if (_mbox_get(mbox, &msg, 1)) {
            printf("Received event: %d\n", msg.content.value);
        }
    }
}

int main() {
    msg_t queue[QUEUE_SIZE];
    mbox_t mbox;
    mbox_init(&mbox, queue, QUEUE_SIZE);

    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, &mbox, "sender");
    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, &mbox, "receiver");

    return 0;
}
