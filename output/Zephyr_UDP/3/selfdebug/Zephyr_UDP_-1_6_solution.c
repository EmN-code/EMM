
#include <zephyr.h>
#include <net/socket.h>
#include <logging/log.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"

LOG_MODULE_REGISTER(udp_client);

static int sock;
static struct sockaddr_in server_addr;

void send_device_state(void)
{
    char buffer[] = DEVICE_STATE;
    int ret;

    ret = sendto(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        LOG_ERR("Failed to send data to server: %d", errno);
    } else {
        LOG_INF("Sent device state: %s", buffer);
    }
}

void main(void)
{
    int ret;

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", errno);
        return;
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    ret = inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    if (ret < 0) {
        LOG_ERR("Invalid address: %d", errno);
        close(sock);
        return;
    }

    // Send device state every 5 seconds
    while (1) {
        send_device_state();
        k_sleep(K_SECONDS(5));
    }
}
