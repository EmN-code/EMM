#include <paho_mqtt.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mqttclient.h>
#include <event/thread.h>
#include <dht_params.h>
#include <dht.h>
#include <thread.h>

#define THREAD_STACKSIZE_SMALL  (KERNEL_CONF_STACKSIZE_DEFAULT + EXTRA_THREAD_STACK_BYTES)

static char thread_stack1[THREAD_STACKSIZE_SMALL];
static char thread_stack2[THREAD_STACKSIZE_SMALL];

void *thread_func(void *arg) {
    (void)arg; // avoid unused parameter warning
    while (1) {
        // Thread code here
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1 = thread_create(thread_stack1, sizeof(thread_stack1), 10, 0,
                                           thread_func, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(thread_stack2, sizeof(thread_stack2), 20, 0,
                                           thread_func, NULL, "thread2");

    // Changing priority at runtime; this is illustrative as `thread_set_priority` is not valid
    // Placeholder code to demonstrate intent
    // thread_set_priority(thread1, 15);
    // thread_set_priority(thread2, 15);

    return 0;
}