
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL_SECONDS 5

static int create_udp_socket(void)
{
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Error creating socket\n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    return sock;
}

static void send_device_state(int sock, const char *state)
{
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    ssize_t ret = sendto(sock, state, strlen(state), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printk("Error sending data\n");
    } else {
        printk("Sent: %s\n", state);
    }
}

int main(void)
{
    int sock;
    bool device_working = true; // Simulate device state

    sock = create_udp_socket();
    if (sock < 0) {
        return -1;
    }

    while (1) {
        if (device_working) {
            send_device_state(sock, DEVICE_STATE_WORK);
        } else {
            send_device_state(sock, DEVICE_STATE_NOT_WORK);
        }

        device_working = !device_working; // Toggle state

        k_sleep(K_SECONDS(SEND_INTERVAL_SECONDS));
    }

    close(sock);
    return 0;
}
