#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL K_SECONDS(5)  // 5 seconds interval
#define DEVICE_STATE "work"

void main(void)
{
    struct sockaddr_in server_addr;
    int sockfd;
    int ret;

    // Configure the server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        printk("Failed to create UDP socket\n");
        return;
    }

    while (1) {
        // Send device state to the server
        ret = sendto(sockfd, DEVICE_STATE, sizeof(DEVICE_STATE), 0,
                     (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (ret < 0) {
            printk("Failed to send data\n");
        } else {
            printk("Data sent successfully\n");
        }

        // Wait for the next send interval
        k_sleep(SEND_INTERVAL);
    }

    // Close the socket - for good practice, though the loop is infinite
    close(sockfd);
}
