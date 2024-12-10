#include <zephyr/types.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/wifi.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/device.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/random/random.h>
#include <zephyr/net/buf.h>
#include <zephyr/toolchain.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/stats/stats.h>
#include <zephyr/pm/state.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/posix/poll.h>
#include <zephyr/sys/slist.h>
#include <zephyr/syscalls/i2c.h>
#include <sys/time.h>
#include <zephyr/init.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/devicetree.h>
#include <time.h>
#include <zephyr/sys/util.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/net/websocket.h>

#define SERVER_ADDR "47.102.103.1"
#define COAP_PORT 5683

int main(void) {
    struct sockaddr_in server_addr;
    int sock;
    char msg[] = "Hello, COAP Cloud";
    struct coap_packet request;
    uint8_t request_buffer[256];

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(COAP_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);
    
    if (coap_packet_init(&request, request_buffer, sizeof(request_buffer), 1, COAP_TYPE_CON, 0, NULL, COAP_METHOD_POST, coap_next_id()) < 0) {
        printk("Failed to init CoAP request\n");
        close(sock);
        return -1;
    }

    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, (uint8_t *) "coap/test", strlen("coap/test"));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)msg, strlen(msg));

    if (sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    printk("CoAP request sent successfully\n");
    close(sock);
    return 0;
}