
#include <zephyr/sys/device_mmio.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/slist.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/wifi.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/websocket.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/net/buf.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/device.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/socket.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/posix/poll.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/random/random.h>
#include <zephyr/devicetree.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/device.h>
#include <zephyr/types.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

int main(void) {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[128];

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        printk("Invalid address/ Address not supported\n");
        close(sock);
        return -1;
    }

    // Prepare the CoAP packet
    struct coap_packet packet;
    coap_packet_init(&packet, buffer, sizeof(buffer), COAP_VERSION_1, COAP_TYPE_CON, 0,
                     coap_next_token(), COAP_METHOD_POST, coap_next_id());

    // Add URI path option
    coap_packet_append_option(&packet, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));

    // Add payload marker and payload
    coap_packet_append_payload_marker(&packet);
    coap_packet_append_payload(&packet, PAYLOAD, strlen(PAYLOAD));

    // Send the CoAP packet
    if (sendto(sock, packet.data, packet.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    printk("CoAP request sent successfully\n");

    // Close the socket
    close(sock);

    return 0;
}
