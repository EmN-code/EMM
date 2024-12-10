#include <zephyr/kernel.h>
#include <zephyr/linker/sections.h>
#include <zephyr/sys/slist.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/random/random.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/fs/nvs.h>

int zsock_inet_pton(int family, const char *src, void *dst);

int main(void) {
    struct sockaddr_in server_addr;
    int sock;
    char payload[] = "Hello, COAP Cloud";

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5683);
    inet_pton(AF_INET, "47.102.103.1", &server_addr.sin_addr);

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock < 0) {
        return -1;
    }

    sendto(sock, payload, sizeof(payload), 0,
           (struct sockaddr *)&server_addr, sizeof(server_addr));
           
    close(sock);

    return 0;
}
