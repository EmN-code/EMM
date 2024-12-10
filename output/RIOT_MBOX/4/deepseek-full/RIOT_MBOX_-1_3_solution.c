
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

// Simulated RIOT-like structures and functions
typedef struct {
    void (*callback)(void *);
    void *arg;
} evtimer_event_t;

typedef struct {
    evtimer_event_t *queue;
    unsigned int queue_size;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} mbox_t;

typedef struct {
    int type;
    void *content;
} msg_t;

// Simulated RIOT-like functions
void mbox_init(mbox_t *mbox, evtimer_event_t *queue, unsigned int queue_size) {
    mbox->queue = queue;
    mbox->queue_size = queue_size;
    pthread_mutex_init(&mbox->lock, NULL);
    pthread_cond_init(&mbox->cond, NULL);
}

void evtimer_add_msg(mbox_t *mbox, evtimer_event_t *event) {
    pthread_mutex_lock(&mbox->lock);
    // Simulate adding event to the queue
    pthread_mutex_unlock(&mbox->lock);
    pthread_cond_signal(&mbox->cond);
}

int mbox_get(mbox_t *mbox, evtimer_event_t *event, int timeout) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += timeout;

    pthread_mutex_lock(&mbox->lock);
    int result = pthread_cond_timedwait(&mbox->cond, &mbox->lock, &ts);
    if (result == 0) {
        // Simulate getting event from the queue
        pthread_mutex_unlock(&mbox->lock);
        return 0;
    } else {
        pthread_mutex_unlock(&mbox->lock);
        return -1;
    }
}

// Simulated callback function
void callback_function(void *arg) {
    printf("Callback triggered with arg: %s\n", (char *)arg);
}

// Simulated sender thread
void *sender_thread(void *arg) {
    mbox_t *mbox = (mbox_t *)arg;
    evtimer_event_t event;
    event.callback = callback_function;
    event.arg = "Hello from sender!";

    while (1) {
        evtimer_add_msg(mbox, &event);
        sleep(2); // Simulate sending event every 2 seconds
    }
    return NULL;
}

// Simulated receiver thread
void *receiver_thread(void *arg) {
    mbox_t *mbox = (mbox_t *)arg;
    evtimer_event_t event;

    while (1) {
        if (mbox_get(mbox, &event, 5) == 0) {
            event.callback(event.arg);
        } else {
            printf("Timeout waiting for message.\n");
        }
    }
    return NULL;
}

int main() {
    mbox_t mbox;
    evtimer_event_t queue[10];
    mbox_init(&mbox, queue, 10);

    pthread_t sender, receiver;
    pthread_create(&sender, NULL, sender_thread, &mbox);
    pthread_create(&receiver, NULL, receiver_thread, &mbox);

    pthread_join(sender, NULL);
    pthread_join(receiver, NULL);

    return 0;
}
