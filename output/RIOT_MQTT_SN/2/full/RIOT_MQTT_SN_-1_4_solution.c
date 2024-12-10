#include <ztimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <thread.h>
#include <timex.h>
#include <time_units.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <msg.h>
#include <mutex.h>
#include <arpa/inet.h>

// MQTT-SN Settings
#define EMCUTE_ID        "riot"
#define EMCUTE_PRIO      (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS        (16U)
#define TOPIC_MAXLEN     (64U)

// Define MQTT broker and port
static sock_udp_ep_t gw = { .family = AF_INET6, .port = 1883 };
static const char *state_topic = "device/state";

static emcute_topic_t topics[NUMOFSUBS];
static char stack[THREAD_STACKSIZE_DEFAULT];

// Buffer and state variable
static char message[64];
static bool work_state = true;

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run((uint16_t)gw.port, EMCUTE_ID);
    return NULL;  // should never be reached
}

int main(void) {
    // Initialize and connect to the broker
    puts("Starting MQTT-SN client implementation...");
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, "47.102.103.1");
    
    // Start emcute thread
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    // connect to gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway.");
        return 1;
    }

    // Register the topic
    strcpy(topics[0].name, state_topic);
    if (emcute_reg(&topics[0]) != EMCUTE_OK) {
        puts("Error: unable to register topic.");
        return 1;
    }

    // Main publishing loop
    while (1) {
        if (work_state) {
            snprintf(message, sizeof(message), "work");
        } else {
            snprintf(message, sizeof(message), "not work");
        }

        if (emcute_pub(&topics[0], message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish data.");
        } else {
            printf("Published: %s\n", message);
        }

        // Toggle work state and sleep
        work_state = !work_state;
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}