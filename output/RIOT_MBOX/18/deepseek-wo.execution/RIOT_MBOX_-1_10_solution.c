
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <mutex.h>
#include <dht.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <event/thread.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <dht_params.h>

#define MSG_QUEUE_SIZE (16)
#define INTERVAL (1000000)  // 1 second in microseconds

typedef struct {
    msg_t queue[MSG_QUEUE_SIZE];
    atomic_int read_pos;
    atomic_int write_pos;
} mbox_t;

static void sender_thread(void *arg)
{
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;

    while (1) {
        msg.content.value = esp_random();  // Generate a random event
        int next_write = (atomic_load(&mbox->write_pos) + 1) % MSG_QUEUE_SIZE;

        if (next_write != atomic_load(&mbox->read_pos)) {
            mbox->queue[atomic_load(&mbox->write_pos)] = msg;
            atomic_store(&mbox->write_pos, next_write);
        }

        usleep(INTERVAL);  // Sleep for 1 second
    }
}

int main(void)
{
    mbox_t mbox;
    atomic_store(&mbox.read_pos, 0);
    atomic_store(&mbox.write_pos, 0);

    // Create the sender thread
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, &mbox, "sender");

    msg_t msg;
    while (1) {
        int next_read = (atomic_load(&mbox.read_pos) + 1) % MSG_QUEUE_SIZE;

        if (next_read != atomic_load(&mbox.write_pos)) {
            msg = mbox.queue[atomic_load(&mbox.read_pos)];
            atomic_store(&mbox.read_pos, next_read);
            printf("Received event: %u\n", msg.content.value);
        }

        usleep(INTERVAL);  // Sleep for 1 second
    }

    return 0;
}
