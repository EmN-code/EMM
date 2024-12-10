#include <xtimer.h>
#include <net/sock/tcp.h>
#include <arpa/inet.h>
#include <string.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    sock_t sock;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    uint8_t send_buf[] = "work";
    uint8_t recv_buf[64];
    char addr_str[INET_ADDRSTRLEN];

    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, &remote, NULL, 0) < 0) {
        printf("Error creating TCP session\n");
        return 1;
    }

    if (sock_tcp_connect(&sock, &remote, sizeof(remote), 0) < 0) {
        printf("Error connecting to the broker\n");
        return 1;
    }

    while (1) {
        ssize_t res = sock_tcp_write(&sock, send_buf, sizeof(send_buf));
        if (res < 0) {
            printf("Error sending data\n");
        }

        res = sock_tcp_read(&sock, recv_buf, sizeof(recv_buf) - 1);
        if (res > 0) {
            recv_buf[res] = '\0';
            printf("Received: %s\n", recv_buf);
        }

        xtimer_sleep(INTERVAL);
    }

    sock_tcp_disconnect(&sock);
    return 0;
}