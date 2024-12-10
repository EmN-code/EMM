#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define SERVER_PORT 20001
#define SERVER_ADDR "47.102.103.1"
#define SEND_INTERVAL K_SECONDS(5)

void main(void) {
    int sock;
    struct sockaddr_in server_addr;
    const char *work_message = "work";
    const char *not_work_message = "not work";
    bool device_state = true;
    
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket. Error: %d\n", errno);
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    while (1) {
        const char *message = device_state ? work_message : not_work_message;

        if (sendto(sock, message, strlen(message), 0, 
                   (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            printk("Failed to send data. Error: %d\n", errno);
        } else {
            printk("Message sent: %s\n", message);
        }

        device_state = !device_state; // Toggle device state
        k_sleep(SEND_INTERVAL);      // Sleep for 5 seconds
    }

    close(sock);
}