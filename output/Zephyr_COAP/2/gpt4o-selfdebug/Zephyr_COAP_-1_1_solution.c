#include <zephyr/net/coap.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_if.h>
#include <zephyr/kernel.h>

#define COAP_SERVER_ADDR "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MSG "Hello, COAP Cloud"

void main(void) {
    struct sockaddr_in server_addr;
    int sock;
    struct coap_packet request;
    uint8_t request_data[128]; // buffer size for the request packet
    int ret;

    // Configure the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(COAP_SERVER_PORT);
    inet_pton(AF_INET, COAP_SERVER_ADDR, &server_addr.sin_addr);

    // Create a socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    // Initialize the CoAP packet
    ret = coap_packet_init(&request, request_data, sizeof(request_data), 1, COAP_TYPE_CON, 8, coap_next_token(), COAP_METHOD_POST, coap_next_id());
    if (ret < 0) {
        printk("Failed to initialize CoAP packet\n");
        close(sock);
        return;
    }

    // Set the request URI path
    ret = coap_packet_append_option(&request, COAP_OPTION_URI_PATH, COAP_URI_PATH, strlen(COAP_URI_PATH));
    if (ret < 0) {
        printk("Failed to set CoAP URI path\n");
        close(sock);
        return;
    }

    // Set the payload
    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        printk("Failed to append CoAP payload marker\n");
        close(sock);
        return;
    }

    ret = coap_packet_append_payload(&request, (uint8_t*)COAP_MSG, strlen(COAP_MSG));
    if (ret < 0) {
        printk("Failed to append CoAP payload\n");
        close(sock);
        return;
    }

    // Send the CoAP request
    ret = sendto(sock, request.data, request.offset, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return;
    }

    printk("CoAP request sent successfully\n");

    // Clean up
    close(sock);
}

int main() {
    main();
    return 0;
}