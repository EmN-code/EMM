#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "MBQTTClient.h"
#include "dht.h"
#include "dht_params.h"

#define THRD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN)
#define MSG_QUEUE_SIZE    (8)

static char msg_stack[THREAD_STACKSIZE_DEFAULT];
msg_t msg_queue[MSG_QUEUE_SIZE];

static void *msg_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        // Wait to receive a message
        msg_receive(&msg);

        // Check the message type and content here
        if (msg.type == YOUR_MESSAGE_TYPE) {
            printf("Message received: %ld\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create a thread for message handling
    thread_create(msg_stack, sizeof(msg_stack), THRD_PRIORITY_MAIN, 0,
                  msg_thread, NULL, "msg_thread");

    // Send messages periodically (simplified example)
    for (unsigned i = 0;; ++i) {
        msg_t msg;
        msg.type = YOUR_MESSAGE_TYPE;
        msg.content.value = i;

        // Send a message to the message handling thread
        if (msg_send(&msg, thread_getpid_of(msg_thread)) < 0) {
            printf("Error: Could not send message\n");
        }

        // Simulate a delay before sending next message
        usleep(1000000);  // 1 second
    }
    return 0;
}