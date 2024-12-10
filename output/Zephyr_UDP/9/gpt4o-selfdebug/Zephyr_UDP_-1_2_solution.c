#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/kernel.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define MESSAGE_INTERVAL K_SECONDS(5)

static const char *WORK_MESSAGE = "work";
static const char *NOT_WORK_MESSAGE = "not work";
static struct sockaddr_in server_addr;

void main(void) {
    int sock;
    struct sockaddr_in addr;
    char *msg = (/* condition to check work state */) ? WORK_MESSAGE : NOT_WORK_MESSAGE;

    printk("Starting UDP client\n");

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket: %d\n", errno);
        return;
    }

    while (1) {
        // Send message to the server
        if (sendto(sock, msg, strlen(msg), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            printk("Failed to send message: %d\n", errno);
        } else {
            printk("Sent message: %s\n", msg);
        }

        // Wait for the defined interval
        k_sleep(MESSAGE_INTERVAL);
    }

    // Close socket
    close(sock);
}
