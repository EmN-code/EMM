#include <zephyr/net/net_ip.h>  // Net socket and IP
#include <zephyr/net/coap.h>     // CoAP socket API
#include <zephyr/kernel.h>       // Zephyr kernel
#include <zephyr/zephyr.h>       // General Zephyr includes

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define POST_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

tatic void send_coap_post(void) {
    struct sockaddr_in dest_addr;
    int sock;

    // Resolve server address
    zsock_inet_pton(AF_INET, SERVER_ADDR, &dest_addr.sin_addr);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(SERVER_PORT);

    // Initialize CoAP request
    struct coap_packet request;
    uint8_t buf[128];
    coap_packet_init(&request, buf, sizeof(buf), 1, COAP_TYPE_CON, COAP_INIT_TOKEN_LEN);
    coap_packet_append_option_int(&request, COAP_OPTION_URI_PATH, POST_PATH);
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)MESSAGE, strlen(MESSAGE));

    // Send request
    sock = zsock_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    if (zsock_sendto(sock, request.data, request.offset, 0,
            (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
        printk("Failed to send CoAP request\n");
    }

    // Clean up
    zsock_close(sock);
}

void main(void) {
    printk("Sending CoAP POST request\n");
    send_coap_post();
}