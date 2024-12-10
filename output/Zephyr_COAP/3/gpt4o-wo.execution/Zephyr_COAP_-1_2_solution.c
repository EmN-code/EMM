#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/coap.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/syscalls/socket.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

void main(void) {
    printk("CoAP client sample on ESP32\n");

    int sock;
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t request_buffer[128];

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    server_addr.sin_port = htons(SERVER_PORT);

    /* Initialize CoAP packet */
    coap_packet_init(&request, request_buffer, sizeof(request_buffer),
                     COAP_VERSION_1, COAP_TYPE_CON, 0,
                     coap_next_id(), COAP_METHOD_POST, NULL, 0);

    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)MESSAGE, strlen(MESSAGE));

    sendto(sock, request.data, request.offset, 0,
           (struct sockaddr *)&server_addr, sizeof(server_addr));

    printk("Sent CoAP request to %s:%d with path %s\n", SERVER_IP, SERVER_PORT, URI_PATH);

    close(sock);
}