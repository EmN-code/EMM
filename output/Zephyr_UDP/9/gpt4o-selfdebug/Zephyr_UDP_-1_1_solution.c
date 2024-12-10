#include <zephyr/net/socket.h>
#include <zephyr/devices.h>
#include <zephyr/kernel.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL K_SECONDS(5)

void udp_send_state(const char *state) {
    struct sockaddr_in server_addr;
    int sock;
    int ret;

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Error: Unable to create socket\n");
        return;
    }
    
    // Initialize server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Send the state to the server
    ret = sendto(sock, state, strlen(state), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printk("Error: Unable to send data\n");
    } else {
        printk("State sent: %s\n", state);
    }

    // Close the socket
    close(sock);
}

void main() {
    const char *state = "work";

    while (1) {
        udp_send_state(state);
        k_sleep(SEND_INTERVAL);
    }
}