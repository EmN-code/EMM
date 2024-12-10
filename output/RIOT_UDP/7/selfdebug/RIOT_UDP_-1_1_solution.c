#include <xtimer.h>
#include <net/inet/udp.h>
#include <stdio.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL 5U // Interval in seconds

void send_device_state(const char *state) {
    struct udp_pcb pcb;
    struct sockaddr_in server_addr;
    
    // Create a new UDP control block
    if (udp_new(&pcb) != 0) {
        printf("Failed to create UDP socket\n");
        return;
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr.s_addr);
    server_addr.sin_port = htons(SERVER_PORT);

    // Send the state to the server
    if (udp_sendto(&pcb, state, strlen(state) + 1, (struct sockaddr*)&server_addr) < 0) {
        printf("Failed to send state\n");
    } else {
        printf("State sent: %s\n", state);
    }

    // Free the UDP control block
    udp_remove(&pcb);
}

int main(void) {
    while (1) {
        // You can replace "work" and "not work" with actual device states.
        char *device_state = "work"; 

        send_device_state(device_state);

        // Wait for the interval
        xtimer_sleep(INTERVAL);
    }

    return 0;
}