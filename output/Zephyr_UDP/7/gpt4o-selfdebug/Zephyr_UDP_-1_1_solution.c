// Functionality: This code establishes a UDP connection to a server with IP address 47.102.103.1 and port 20001. It sends the device state ('work' or 'not work') every 5 seconds.

#include <zephyr/sys/printk.h> // For printing output to console.
#include <zephyr/net/socket.h> // For socket programming.
#include <zephyr/kernel.h> // For kernel functions like k_sleep.

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL 5000 // in milliseconds

void udp_client_thread(void) {
    int sock;
    struct sockaddr_in server_addr;
    char *msg_work = "work";
    char *msg_not_work = "not work";
    bool device_state = true; // Example device state, toggles every send cycle

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    while (1) {
        const char *message = device_state ? msg_work : msg_not_work;

        // Send message to server
        if (sendto(sock, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            printk("Failed to send message\n");
        } else {
            printk("Sent message: %s\n", message);
        }

        // Toggle device state
        device_state = !device_state;

        // Wait before sending the next message
        k_sleep(K_MSEC(SEND_INTERVAL));
    }

    // Close the socket (not reachable in this loop, but good practice)
    close(sock);
}

void main(void) {
    printk("Starting UDP Client\n");
    udp_client_thread();
}