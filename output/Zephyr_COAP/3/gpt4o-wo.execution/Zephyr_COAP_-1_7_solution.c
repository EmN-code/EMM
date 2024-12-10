#include <zephyr/net/websocket.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/flash.h>  // Required for flash memory operations
#include <zephyr/fs/nvs.h>         // File system and non-volatile storage
#include <zephyr/init.h>           // System initialization
#include <zephyr/kernel.h>         // Main kernel functionalities
#include <zephyr/pm/state.h>       // Power management state
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/poll.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/random/random.h>  // Random number generation
#include <zephyr/stats/stats.h>    // Statistics collection
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/math_extras.h>  // Additional math functions
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/mutex.h>     // Mutex utilities for synchronization
#include <zephyr/sys/printk.h>    // Print utilities
#include <zephyr/sys/slist.h>
#include <zephyr/sys/util.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/i2c.h> // System calls for I2C communication
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/tracing/tracing_macros.h> // Tracing utilities
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

// Main function
typedef void (*coap_callback_t)(void);

int send_coap_request(const char *server_addr, uint16_t server_port, const char *uri_path) {
    int sock;
    struct sockaddr_in dest_addr;
    
    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Error: Failed to create socket\n");
        return -1;
    }

    // Initialize destination address structure
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_addr, &dest_addr.sin_addr);

    // Prepare CoAP packet
    uint8_t request_data[] = "Hello, COAP Cloud";
    uint8_t packet[128];

    // Assume coap_packet is initialized properly
    struct coap_packet coap_pkt;
    coap_packet_init(&coap_pkt, packet, sizeof(packet), 1, COAP_TYPE_CON, (&coap_pkt)->tkl, (const uint8_t *)(&coap_pkt)->token, COAP_METHOD_POST, coap_next_id());

    // Append URI path
    coap_packet_append_option(&coap_pkt, COAP_OPTION_URI_PATH, uri_path, strlen(uri_path));
    // Add payload marker
    coap_packet_append_payload_marker(&coap_pkt);

    // Append payload
    coap_packet_append_payload(&coap_pkt, request_data, strlen(request_data));

    // Send CoAP packet
    ssize_t bytes_sent = sendto(sock, packet, coap_packet_get_len(&coap_pkt), 0,
                                (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (bytes_sent < 0) {
        printk("Error: Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    printk("CoAP request sent to: %s\n", server_addr);

    close(sock);
    return 0;
}

int main(void) {
    printk("Starting CoAP Client\n");

    // Send CoAP request
    send_coap_request(SERVER_ADDR, SERVER_PORT, COAP_URI_PATH);

    return 0;
}