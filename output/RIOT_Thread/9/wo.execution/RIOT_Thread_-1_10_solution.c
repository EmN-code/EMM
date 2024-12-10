// Description: This code demonstrates inter-thread communication using msg_t in RIOT OS. We create two threads with different priorities. One thread sends messages, and the other receives them. The unused variable warning for 'msg' is fixed by removing any unused msg_t variable declaration.

// Including required header files in descending order by their first letter
#include <ztimer.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <mutex.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdbool.h>

#define MSG_QUEUE_SIZE    (8)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t msg_queue[MSG_QUEUE_SIZE];

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    int count = 0;  // A simple counter to mark received messages

    while (1) {
        // Blocking receive
        if (msg_receive(&msg) == 1) { 
          count++;
        }
        // Process the message
        printf("Received message of type: %u. Total messages received: %d\n", msg.type, count);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t receiver_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                               THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating thread\n");
        return 1;
    }

    msg_t msg;
    msg.type = 0x1234;  // Just some dummy message type
    msg.content.value = 42;  // Some dummy content

    while (1) {
        printf("Sending message\n");
        if (msg_send(&msg, receiver_pid) == 1) {
            printf("Message sent successfully\n");
        } else {
            printf("Failed to send message\n");
        }
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}