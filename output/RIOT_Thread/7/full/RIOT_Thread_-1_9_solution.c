#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <thread.h>
#include <time_units.h>
#include <timex.h>
#include <msg.h>
#include <mutex.h>
#include <mbox.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <errno.h>
#include <esp_system.h>
#include <dht.h>
#include <dht_params.h>
#include <esp_wifi.h>
#include <kernel_defines.h>
#include <arpa/inet.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[8];
static kernel_pid_t main_thread_pid;

/* Thread function for sending a message */
void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    while (1) {
        msg_send(&msg, main_thread_pid);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    main_thread_pid = thread_getpid();

    /* Create sender thread */
    kernel_pid_t sender_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    if (sender_pid <= KERNEL_PID_UNDEF) {
        puts("Error: Unable to create sender thread");
        return 1;
    }

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }
    return 0;
}