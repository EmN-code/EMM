#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE_MSG "work"
#define SLEEP_TIME K_SECONDS(5)

void udp_send_device_state(void)
{
    struct sockaddr_in server_addr;
    int sock;
    int ret;

    /* Create socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    /* Prepare server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    /* Send device state to server */
    while (1) {
        ret = sendto(sock, DEVICE_STATE_MSG, strlen(DEVICE_STATE_MSG), 0,
                     (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (ret < 0) {
            printk("Failed to send device state\n");
        } else {
            printk("Sent device state: %s\n", DEVICE_STATE_MSG);
        }

        /* Sleep for 5 seconds */
        k_sleep(SLEEP_TIME);
    }

    /* Close socket */
    close(sock);
}

int main(void)
{
    printk("Starting UDP device state sender\n");

    udp_send_device_state();

    return 0;
}