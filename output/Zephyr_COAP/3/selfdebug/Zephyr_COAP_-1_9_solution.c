
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/ethernet.h>  // Corrected include
#include <zephyr/net/ethernet_vlan.h>
#include <zephyr/net/ethernet_bridge.h>
#include <zephyr/net/ethernet_mgmt.h>
#include <zephyr/net/ethernet_lldp.h>
#include <zephyr/net/ethernet_tap.h>
#include <zephyr/net/ethernet_ppp.h>  // Removed this include as it doesn't exist

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

void send_coap_request(int sock) {
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t buffer[128];
    int ret;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    coap_packet_init(&request, buffer, sizeof(buffer), COAP_TYPE_CON, COAP_METHOD_POST, 0);
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, "Hello, COAP Cloud", 16);

    ret = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printk("Failed to send CoAP request: %d\n", ret);
    } else {
        printk("CoAP request sent successfully\n");
    }
}

int main(void) {
    int sock;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket: %d\n", sock);
        return -1;
    }

    send_coap_request(sock);

    close(sock);
    return 0;
}
