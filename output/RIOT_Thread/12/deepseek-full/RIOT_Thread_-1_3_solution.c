
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define STACK_SIZE 1024
#define NUM_MESSAGES 10

typedef struct {
    int sender_pid;
    int type;
    union {
        void *ptr;
        int value;
    } content;
} msg_t;

typedef struct {
    msg_t *queue;
    int size;
    int head;
    int tail;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} msg_queue_t;

msg_queue_t msg_queue;

void msg_queue_init(msg_queue_t *q, msg_t *array, int num) {
    q->queue = array;
    q->size = num;
    q->head = 0;
    q->tail = 0;
    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);
}

int msg_queue_send(msg_queue_t *q, msg_t *m) {
    pthread_mutex_lock(&q->lock);
    while (((q->tail + 1) % q->size) == q->head) {
        pthread_cond_wait(&q->not_full, &q->lock);
    }
    q->queue[q->tail] = *m;
    q->tail = (q->tail + 1) % q->size;
    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->lock);
    return 1;
}

int msg_queue_receive(msg_queue_t *q, msg_t *m) {
    pthread_mutex_lock(&q->lock);
    while (q->head == q->tail) {
        pthread_cond_wait(&q->not_empty, &q->lock);
    }
    *m = q->queue[q->head];
    q->head = (q->head + 1) % q->size;
    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->lock);
    return 1;
}

void *sender_thread(void *arg) {
    msg_t msg;
    for (int i = 0; i < NUM_MESSAGES; i++) {
        msg.sender_pid = pthread_self();
        msg.type = i;
        msg.content.value = i * 10;
        msg_queue_send(&msg_queue, &msg);
        printf("Sender: Sent message %d\n", i);
        sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    msg_t msg;
    for (int i = 0; i < NUM_MESSAGES; i++) {
        msg_queue_receive(&msg_queue, &msg);
        printf("Receiver: Received message %d with value %d\n", msg.type, msg.content.value);
    }
    return NULL;
}

int main() {
    pthread_t sender, receiver;
    msg_t msg_array[NUM_MESSAGES];

    msg_queue_init(&msg_queue, msg_array, NUM_MESSAGES);

    pthread_create(&sender, NULL, sender_thread, NULL);
    pthread_create(&receiver, NULL, receiver_thread, NULL);

    pthread_join(sender, NULL);
    pthread_join(receiver, NULL);

    return 0;
}
