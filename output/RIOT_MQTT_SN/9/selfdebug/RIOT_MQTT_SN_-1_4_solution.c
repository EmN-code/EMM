#include <stdio.h>
#include <string.h>
#include <xtimer.h> // Needed for xtimer_sleep
#include <net/sock/udp.h> // Includes UDP networking functionalities
#include <net/emcute.h> // For MQTT-SN related functionalities

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define PUB_INTERVAL (5U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_topic_t topic;
    topic.name = "state";
    topic.id = 0;
    
    while (1) {
        // Connect to the MQTT-SN broker
        sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
        inet_pton(AF_INET, SERVER_ADDR, &gw.addr);
        emcute_con(&gw, true, NULL, NULL, 0, 0);
        
        // Publish state
        const char *state_msg = "work"; // You can switch between "work" or "not work"
        emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0);
        emcute_discon();

        // Sleep for interval
        xtimer_sleep(PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    // Initialize and kick-off the emcute thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");
    
    msg_init_queue(queue, 8);
    
    while (1) {
        // Main loop doing nothing, as work is done in the emcute_thread
        xtimer_sleep(1);
    }

    return 0;
}