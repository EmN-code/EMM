#include <zephyr/net/socket.h> // for socket operations
#include <zephyr/net/coap.h> // for CoAP utility functions
#include <zephyr/kernel.h> // for kernel functionalities

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

void send_coap_request(void) {
    int sock;
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t buffer[256];

    // Set up the server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Create a socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    // Initialize the CoAP request
    if (coap_packet_init(&request, buffer, sizeof(buffer), 1, COAP_TYPE_CON, COAP_CODE_POST, coap_next_id()) < 0) {
        printk("Failed to initialize CoAP request\n");
        close(sock);
        return;
    }

    // Set the URI path
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));

    // Append the message to the CoAP packet
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)MESSAGE, strlen(MESSAGE));

    // Send the CoAP request
    if (sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printk("Failed to send CoAP request\n");
    } else {
        printk("CoAP request sent successfully\n");
    }

    // Close the socket
    close(sock);
}

int main(void) {
    printk("Starting CoAP client\n");
    // Send the CoAP request
    send_coap_request();
    return 0;
}