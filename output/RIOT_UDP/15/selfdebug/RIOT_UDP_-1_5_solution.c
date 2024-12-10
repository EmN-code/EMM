#include <ztimer.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    const char *server_ip = "47.102.103.1";
    uint16_t server_port = 20001;
    struct sockaddr_in server_addr;
    sock_udp_t sock;
    sock_udp_ep_t remote;
    int res;
    char *state_working = "work";
    char *state_not_working = "not work";
    char *current_state;

    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    sock_udp_create(&sock, NULL, NULL, 0);

    remote.family = AF_INET;
    remote.port = server_port;
    memcpy(&remote.addr.ipv4, &server_addr.sin_addr, sizeof(remote.addr.ipv4));

    while (1) {
        current_state = (/* condition */) ? state_working : state_not_working;
        res = sock_udp_send(&sock, current_state, strlen(current_state), &remote);
        if (res < 0) {
            printf("Error sending data: %d\n", res);
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    sock_udp_close(&sock);
    return 0;
}