
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

int main(void) {
    int sock;
    struct sockaddr_in server_addr;
    struct coap_packet cpkt;
    uint8_t buffer[128];
    int ret;

    // Initialize the socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Initialize the CoAP packet
    ret = coap_packet_init(&cpkt, buffer, sizeof(buffer), COAP_VERSION_1, COAP_TYPE_CON, 0, NULL, COAP_METHOD_POST, 0);
    if (ret < 0) {
        printk("Failed to initialize CoAP packet\n");
        close(sock);
        return -1;
    }

    // Append the URI path option
    ret = coap_packet_append_option(&cpkt, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));
    if (ret < 0) {
        printk("Failed to append URI path option\n");
        close(sock);
        return -1;
    }

    // Append the payload marker and payload
    ret = coap_packet_append_payload_marker(&cpkt);
    if (ret < 0) {
        printk("Failed to append payload marker\n");
        close(sock);
        return -1;
    }

    ret = coap_packet_append_payload(&cpkt, MESSAGE, strlen(MESSAGE));
    if (ret < 0) {
        printk("Failed to append payload\n");
        close(sock);
        return -1;
    }

    // Send the CoAP request
    ret = sendto(sock, buffer, cpkt.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    printk("CoAP request sent successfully\n");

    close(sock);
    return 0;
}