
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define QUEUE_SIZE 8
#define PRIORITY 5
#define STACK_SIZE 512

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

static inline void mbox_init(mbox_t *mbox, msg_t *queue, unsigned int queue_size) {
    mbox->queue = queue;
    mbox->queue_size = queue_size;
    mbox->read_pos = 0;
    mbox->write_pos = 0;
}

int _mbox_get(mbox_t *mbox, msg_t *msg, int blocking) {
    (void)blocking; // Suppress unused parameter warning
    if (mbox->read_pos == mbox->write_pos) {
        return 0; // Queue is empty
    }
    *msg = mbox->queue[mbox->read_pos];
    mbox->read_pos = (mbox->read_pos + 1) % mbox->queue_size;
    return 1;
}

int msg_reply(msg_t *m, msg_t *reply) {
    if (m == NULL || reply == NULL) {
        return -1;
    }
    reply->sender_pid = m->sender_pid;
    return 1;
}

int msg_receive(msg_t *m) {
    while (1) {
        for (int i = 0; i < QUEUE_SIZE; i++) {
            if (m->type != 0) {
                return 1;
            }
        }
        usleep(1000); // Sleep for a short time to avoid busy-waiting
    }
}

int msg_send_receive(msg_t *m, msg_t *reply, kernel_pid_t target_pid) {
    (void)target_pid; // Suppress unused parameter warning
    if (m == NULL || reply == NULL) {
        return -1;
    }
    reply->sender_pid = m->sender_pid;
    return 1;
}

void msg_init_queue(msg_t *array, int num) {
    for (int i = 0; i < num; i++) {
        array[i].type = 0;
    }
}

void sender_thread(void *arg) {
    mbox_t *mbox = (mbox_t *)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 1234;

    while (1) {
        _mbox_get(mbox, &msg, 1);
        printf("Received message with value: %u\n", (unsigned int)msg.content.value);
        usleep(1000000); // Sleep for 1 second
    }
}

int main() {
    msg_t msg_queue[QUEUE_SIZE];
    mbox_t mbox;
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    char stack[STACK_SIZE];
    thread_create(stack, STACK_SIZE, PRIORITY, 0, (thread_task_func_t)sender_thread, &mbox, "sender_thread");

    msg_t msg;
    msg.type = 1;
    msg.content.value = 5678;

    while (1) {
        _mbox_get(&mbox, &msg, 1);
        printf("Main thread received message with value: %u\n", (unsigned int)msg.content.value);
        usleep(2000000); // Sleep for 2 seconds
    }

    return 0;
}
