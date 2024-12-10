#include <esp_wifi.h>
#include <esp_system.h>
#include <sys/socket.h>
#include <stdint.h>
#include <netinet/in.h>
#include <xtimer.h>

#define THREAD_STACKSIZE_MAIN (4096)
#define THREAD_PRIORITY_MAIN (10)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;
    
    kernel_pid_t receiver_pid = (kernel_pid_t)arg;
    msg_send(&msg, receiver_pid);
    return NULL;
}

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    
    msg_receive(&msg);
    printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    return NULL;
}

int main(void) {
    kernel_pid_t sender_pid, receiver_pid;
    
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_MAIN, 0,
                                 receiver_thread, NULL, "receiver");

    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               THREAD_PRIORITY_MAIN + 1, 0,
                               sender_thread, (void *)receiver_pid, "sender");

    return 0;
}