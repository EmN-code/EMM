# include <stdio.h> 
# include <string.h>
# include <pthread.h>
# include <semaphore.h>
# include <stdlib.h> 

// Define the structure for a message
#define MAX_MESSAGES 10
#define MSG_SIZE 128

typedef struct {
    char messages[MAX_MESSAGES][MSG_SIZE];
    int count;
    sem_t sem;
    pthread_mutex_t lock;
} mbox_t;

void mbox_init(mbox_t *mbox) {
    mbox->count = 0;
    sem_init(&mbox->sem, 0, 0);
    pthread_mutex_init(&mbox->lock, NULL);
}

void mbox_put(mbox_t *mbox, const char *msg) {
    pthread_mutex_lock(&mbox->lock);

    if (mbox->count < MAX_MESSAGES) {
        strncpy(mbox->messages[mbox->count], msg, MSG_SIZE);
        mbox->count++;
        sem_post(&mbox->sem);
    }

    pthread_mutex_unlock(&mbox->lock);
}

char* mbox_get(mbox_t *mbox) {
    sem_wait(&mbox->sem);

    pthread_mutex_lock(&mbox->lock);

    char *msg = NULL;
    if (mbox->count > 0) {
        msg = malloc(MSG_SIZE);
        strncpy(msg, mbox->messages[0], MSG_SIZE);

        // Shift messages
        for (int i = 0; i < mbox->count - 1; i++) {
            strncpy(mbox->messages[i], mbox->messages[i + 1], MSG_SIZE);
        }
        mbox->count--;
    }

    pthread_mutex_unlock(&mbox->lock);

    return msg;
}

void* sender_thread(void *arg) {
    mbox_t *mbox = (mbox_t*)arg;
    int counter = 0;

    while (1) {
        char message[MSG_SIZE];
        sprintf(message, "Event %d", counter++);
        mbox_put(mbox, message);
        sleep(2); // send event every 2 seconds
    }
}

void* receiver_thread(void *arg) {
    mbox_t *mbox = (mbox_t*)arg;

    while (1) {
        char *message = mbox_get(mbox);
        if (message != NULL) {
            printf("Received: %s\n", message);
            free(message);
        }
    }
}

int main() {
    mbox_t mbox;
    mbox_init(&mbox);

    pthread_t sender, receiver;
    pthread_create(&sender, NULL, sender_thread, (void*)&mbox);
    pthread_create(&receiver, NULL, receiver_thread, (void*)&mbox);

    pthread_join(sender, NULL);
    pthread_join(receiver, NULL);

    return 0;
}