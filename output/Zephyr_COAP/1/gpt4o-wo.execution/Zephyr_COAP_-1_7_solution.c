#include <zephyr/net/tls_credentials.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/websocket.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/poll.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/random/random.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/util.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/types.h>
#include <stdio.h>
#include <string.h>

/* This example assumes the application has been configured to connect to a COAP server. */

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static int coap_client_send_request(const char *server_addr, uint16_t server_port, const char *uri_path, const char *payload) {
    int sock;
    struct sockaddr_in addr = {0};

    /* Initialize a socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_addr, &addr.sin_addr);

    uint8_t data[128];
    struct coap_packet packet;

    /* Initialize CoAP packet */
    coap_packet_init(&packet, data, sizeof(data), 1, COAP_TYPE_CON, 0, NULL, COAP_METHOD_POST, coap_next_id());
    coap_packet_append_option(&packet, COAP_OPTION_URI_PATH, (uint8_t *)uri_path, strlen(uri_path));
    coap_packet_append_payload_marker(&packet);
    coap_packet_append_payload(&packet, (uint8_t *)payload, strlen(payload));

    /* Send CoAP request */
    if(sendto(sock, packet.data, packet.offset, 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    close(sock);
    return 0;
}

int main() {
    printk("Sending CoAP request\n");
    coap_client_send_request(SERVER_ADDR, SERVER_PORT, COAP_URI_PATH, "Hello, COAP Cloud");
    return 0;
}
