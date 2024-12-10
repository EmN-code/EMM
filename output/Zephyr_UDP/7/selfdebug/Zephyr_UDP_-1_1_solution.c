
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define DEVICE_STATE_MSG_LEN 4
#define SEND_INTERVAL_SECONDS 5

LOG_MODULE_REGISTER(main);

int main(void) {
    int sock;
    struct sockaddr_in server_addr;
    int ret;

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket (%d)", sock);
        return -1;
    }

    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    while (1) {
        // Send the device state message
        ret = sendto(sock, DEVICE_STATE_MSG, DEVICE_STATE_MSG_LEN, 0,
                     (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (ret < 0) {
            LOG_ERR("Failed to send data (%d)", ret);
        } else {
            LOG_INF("Sent device state: %s", DEVICE_STATE_MSG);
        }

        // Wait for 5 seconds before sending the next message
        k_sleep(K_SECONDS(SEND_INTERVAL_SECONDS));
    }

    // Close the socket (this part is unreachable in this example)
    close(sock);

    return 0;
}
