#include <zephyr/toolchain.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/types.h>
#include <zephyr/toolchain.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/buf.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/posix/poll.h>
#include <zephyr/pm/state.h>
#include <zephyr/init.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/kernel.h>
#include <zephyr/random/random.h>
#include <zephyr/stats/stats.h>
#include <zephyr/storage/flash_map.h>

int main() {
    int sock;
    struct sockaddr_in dest_addr;
    struct coap_packet request;
    uint8_t request_data[128];
    const char* payload = "Hello, COAP Cloud";
    const char* path = "/coap/test";

    /* Create socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    /* Setup destination address */
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(5683);
    inet_pton(AF_INET, "47.102.103.1", &dest_addr.sin_addr);

    /* Initialize CoAP packet with correct arguments */
    coap_packet_init(&request, request_data, sizeof(request_data), 1, COAP_TYPE_CON, 8, coap_next_id(), COAP_METHOD_POST, strlen(path));
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, path, strlen(path));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)payload, strlen(payload));

    /* Send CoAP packet */
    if (sendto(sock, request_data, request.offset, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
        printk("Failed to send data\n");
        close(sock);
        return -1;
    }

    /* Close socket */
    close(sock);
    return 0;
}