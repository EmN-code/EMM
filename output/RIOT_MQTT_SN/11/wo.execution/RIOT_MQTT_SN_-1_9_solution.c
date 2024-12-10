#include <unistd.h>       // Needed for using usleep for delays
#include <stdlib.h>       // General utilities
#include <stdio.h>        // Standard I/O routines
#include <esp_wifi.h>     // ESP32 Wi-Fi functions
#include <esp_system.h>   // ESP32 system functions
#include <stdatomic.h>    // Atomic operations
#include <stdbool.h>      // Boolean functions
#include <stddef.h>       // Standard definitions
#include <timex.h>        // Time functions and related structures
#include <netinet/in.h>   // Internet address family
#include <arpa/inet.h>    // Definitions for internet operations
#include <net/emcute.h>   // emcute types and functions
#include <mqemcute_reg() /emcute messaging functions
#include <msg.h>          // Messaging functions for thread communication
#include <thread.h>       // Thread functionality
#include <event/thread.h> // Thread event functionality

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define DEVICE_ID           "esp32"
#define EMCUTE_ID          1
#define SERVER_IP           "47.102.103.1"
#define SERVER_PORT         1883
#define TOPIC_NAME          "device/state"
#define PUBLISH_INTERVAL    (5U * US_PER_SEC)  // 5 seconds

// Main function to initiate the MQTT-SN connection and publish data
typedef struct sockaddr_in sockaddr_in;
struct sockaddr_in6 remote;
char message[64];
msg_t main_msg_queue[8];

static void *_emcute_thread(void *arg) {
    (void)arg;
    emcute_run(SERVER_PORT, DEVICE_ID);
    return NULL; // should never be reached
}

int main(void) {
    inet_pton(AF_INET, SERVER_IP, &remote.sin_addr.s_addr);
    remote.sin_family = AF_INET;
    remote.sin_port = htons(SERVER_PORT);

    msg_init_queue(main_msg_queue, ARRAY_SIZE(main_msg_queue));
    thread_create(xtimer, XTIMER_STACKSIZE_DEFAULT, EMCUTE_PRIO, 0, &emcute_thread,
                  NULL, "emcute");

    emcute_con((sock_udp_ep_t *)&remote, 0, NULL, NULL, 0, 0);

    // Register the topic to publish
    emcute_topic_t t;
    strncpy(t.name, TOPIC_NAME, sizeof(t.name));
    emcute_reg(&t);

    while(1) {
        strcpy(message, "work");
        emcute_pub(&t, message, strlen(message), EMCUTE_QOS_0);
        usleep(PUBLISH_INTERVAL);
    }

    return 0;
}