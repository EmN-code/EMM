#include <zephyr/syscalls/i2c.h>
#include <zephyr/sys/slist.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/stats/stats.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/toolchain.h>
#include <zephyr/time.h>
#include <zephyr/sys/util.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/random/random.h>
#include <zephyr/pm/state.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/coap.h>
#include <zephyr/linker/sections.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/init.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/poll.h>
#include <zephyr/kernel.h>
#include <zephyr/fs/nvs.h>
#include <sys/types.h>

// Functionality setup for CoAP on ESP32
#include <net/coap.h>
#include <net/socket.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define DATA_TO_SEND "Hello, COAP Cloud"

static struct sockaddr_in server_addr;
static int sock;

// Initialize the CoAP message
static int prepare_coap_request(struct coap_packet *request) {
    uint8_t payload[] = DATA_TO_SEND;
    coap_packet_init(request, ...);  // Initialize packet with details
    coap_packet_append_option(request, COAP_OPTION_URI_PATH, (uint8_t *)URI_PATH, strlen(URI_PATH)); // Add URI option
    coap_packet_append_payload_marker(request);
    coap_packet_append_payload(request, payload, sizeof(payload) - 1);
    return 0;
}

// Send function for CoAP message
static void send_coap_request(void) {
    struct coap_packet request;
    struct sockaddr_in dest_addr;
    int r;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &dest_addr.sin_addr);

    prepare_coap_request(&request); // Prepare the CoAP request

    r = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (r < 0) {
        printk("Failed to send CoAP request\n");
    }

    close(sock);
}

int main(void) {
    printk("Starting CoAP client\n");
    send_coap_request();
    return 0;
}