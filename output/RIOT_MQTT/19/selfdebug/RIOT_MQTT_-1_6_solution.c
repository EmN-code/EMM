// Let's fix the errors mentioned: 
// 1. The preprocessing directive should be '#include', not '#Include'.
// 2. Ensure all included headers are correctly named and spelled.

// Description: This program is meant to connect to a MQTT broker at '47.102.103.1:1883' using the ESP32 with RIOT OS. It will periodically send a state message every 5 seconds indicating "work" or "not work" without relying on the emcute library.

#include <thread.h>
#include <stdio.h>
#include <string.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define SERVER_ADDR  "47.102.103.1"
#define SERVER_PORT  1883
#define MQTT_TOPIC   "device/state"
#define WORK_STATE   "work"
#define NOT_WORK_STATE "not work"
#define PUBLISH_INTERVAL (5U * US_PER_SEC) // 5 seconds interval

static sock_udp_ep_t remote;

void *publish_thread(void *arg) {
    (void)arg;
    sock_udp_t sock;
    int res;
    char *state = WORK_STATE;
    
    // Prepare remote endpoint
    remote.family = AF_INET;
    res = inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);
    if (res != 1) {
        printf("Failed to convert IP address\n");
        return NULL;
    }
    remote.port = SERVER_PORT;

    // Initialize UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return NULL;
    }

    while (1) {
        // Toggle work state
        state = (strcmp(state, WORK_STATE) == 0) ? NOT_WORK_STATE : WORK_STATE;
        // Send the state
        if (sock_udp_send(&sock, state, strlen(state), &remote) < 0) {
            printf("Error sending UDP packet\n");
        } else {
            printf("Published state: %s\n", state);
        }

        // Sleep for 5 seconds
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    // Clean up socket (in practice, this code will not be reached)
    sock_udp_close(&sock);
    return NULL;
}

int main(void) {
    // This creates a separate thread for publishing to the MQTT broker
    static char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, publish_thread, NULL, "publish_thread");
    return 0;
}