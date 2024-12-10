#include <xtimer.h>
#include <sys/bytes.h>
#include <evtimer.h>
#include <kernel_defines.h>

#define INTERVAL (5000000U)    // 5 seconds
#define MBOX_SIZE (8)          // message box queue size

static mbox_t mbox;
static msg_t queue[MBOX_SIZE];

void sender_task(void) {
    msg_t msg;
    uint8_t data;

    while (1) {
        random_bytes(&data, sizeof(data));
        msg.content.value = data;

        if (mbox_try_put(&mbox, &msg) != 1) {
            printf("Failed to send message\n");
        }

        xtimer_usleep(INTERVAL);
    }
}

int main(void) {
    char stack[THREAD_STACKSIZE_DEFAULT];

    mbox_init(&mbox, queue, MBOX_SIZE);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, 
                  sender_task, NULL, "sender");

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received message with value: %u\n", (unsigned)msg.content.value);
    }

    return 0;
}