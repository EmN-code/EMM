
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <evtimer_msg.h>
#include <xtimer.h>
#include <msg.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUFFER_SIZE 128
#define DELAY_SECONDS 5

static sock_udp_t sock;
static sock_udp_ep_t remote;
static char buffer[BUFFER_SIZE];

static void send_state(void) {
    static bool state = false;
    const char *message = state ? "work" : "not work";
    state = !state;

    if (sock_udp_send(&sock, message, strlen(message), &remote) < 0) {
        puts("Failed to send message");
    } else {
        printf("Sent: %s\n", message);
    }
}

static void timer_callback(void *arg) {
    (void)arg;
    send_state();
    xtimer_set(&timer, DELAY_SECONDS * US_PER_SEC);
}

static xtimer_t timer;

int main(void) {
    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Invalid server IP address");
        return 1;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Failed to create UDP socket");
        return 1;
    }

    // Set up the timer
    xtimer_init(&timer);
    xtimer_set(&timer, DELAY_SECONDS * US_PER_SEC);

    // Main loop
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        timer_callback(NULL);
    }

    // Clean up
    sock_udp_close(&sock);
    return 0;
}
