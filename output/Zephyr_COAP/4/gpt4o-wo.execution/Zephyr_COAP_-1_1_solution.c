#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>

#define SERVER_HOST "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static int send_coap_post_request(void) {
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t request_data[128];
    int sock;
    ssize_t bytes_sent;

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    // Define server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_HOST, &server_addr.sin_addr);

    // Initialize CoAP packet for POST
    coap_packet_init(&request, request_data, sizeof(request_data), 1, COAP_TYPE_CON, 8, coap_next_token(),
                     COAP_METHOD_POST, coap_next_id());

    // Set URI path
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, COAP_URI_PATH, strlen(COAP_URI_PATH));

    // Set payload
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    // Send CoAP request
    bytes_sent = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bytes_sent < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    printk("CoAP POST request sent\n");
    close(sock);
    return 0;
}

int main(void) {
    printk("Starting CoAP POST request example\n");
    if (send_coap_post_request() == 0) {
        printk("CoAP POST request completed successfully\n");
    } else {
        printk("CoAP POST request failed\n");
    }
    return 0;
}