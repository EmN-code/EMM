
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/util.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/pm/state.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/stats/stats.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/linker/sections.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/net/websocket.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/poll.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

void send_coap_request(int sock, const struct sockaddr *server_addr) {
    struct coap_packet request;
    uint8_t buffer[128];
    int ret;

    ret = coap_packet_init(&request, buffer, sizeof(buffer), COAP_VERSION_1, COAP_TYPE_CON, 0, 0, coap_next_token(), COAP_METHOD_POST, coap_next_id());
    if (ret < 0) {
        printk("Failed to initialize CoAP packet\n");
        return;
    }

    ret = coap_packet_append_option(&request, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));
    if (ret < 0) {
        printk("Failed to append URI path option\n");
        return;
    }

    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        printk("Failed to append payload marker\n");
        return;
    }

    ret = coap_packet_append_payload(&request, MESSAGE, strlen(MESSAGE));
    if (ret < 0) {
        printk("Failed to append payload\n");
        return;
    }

    ret = sendto(sock, request.data, request.offset, 0, server_addr, sizeof(struct sockaddr_in));
    if (ret < 0) {
        printk("Failed to send CoAP request\n");
        return;
    }

    printk("CoAP request sent successfully\n");
}

int main(void) {
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    send_coap_request(sock, (struct sockaddr *)&server_addr);

    close(sock);
    return 0;
}