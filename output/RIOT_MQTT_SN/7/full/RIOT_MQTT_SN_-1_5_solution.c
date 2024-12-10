#include "ztimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stddef.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "msg.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "esp_system.h"

// Configuration
#define EMCUTE_ID           "emcute_client"
#define EMCUTE_PORT         1883
#define BROKER_IP_ADDR      "47.102.103.1"
#define PUB_INTERVAL        (5U * 1000U * 1000U)

// Message buffer configuration
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define STACKSIZE           (THREAD_STACKSIZE_MAIN)
static char stack[STACKSIZE];

// Function prototypes
static void *emcute_thread(void *arg);

// Main function
int main(void) {
    msg_t queue[8];
    msg_init_queue(queue, 8);
    
    // Start the emcute thread
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute_thread");

    return 0;
}

// Thread function for emcute
static void *emcute_thread(void *arg) {
    (void)arg;

    // Initialize endpoint for broker
    sock_udp_ep_t remote;
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    inet_pton(AF_INET, BROKER_IP_ADDR, &remote.addr.ipv4);
    remote.port = EMCUTE_PORT;

    while (1) {
        // Connection logic with retry
        int res;
        do {
            res = emcute_con(&remote, true, NULL, NULL, 0, 0);
            if (res != EMCUTE_OK) {
                printf("Failed to connect to broker. Retrying...\n");
                ztimer_sleep(ZTIMER_USEC, 1000000);
            }
        } while (res != EMCUTE_OK);

        // Publishing the state
        emcute_topic_t topic;
        topic.name = "device/status";
        emcute_reg(&topic);  // Assuming the topic is registered every time

        const char *state = "work";
        if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
            printf("Publication failed\n");
        } else {
            printf("State published successfully: %s\n", state);
        }

        // Disconnect after publishing
        emcute_discon();

        // Sleep before next iteration
        ztimer_sleep(ZTIMER_USEC, PUB_INTERVAL);
    }
    return NULL;
}