#include <zephyr/net/net_ip.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_PATH "/coap/test"
#define COAP_PORT 5683
#define COAP_MSG_CONTENT "Hello, COAP Cloud"

void main(void) {
    struct sockaddr_in server;
    struct coap_packet request;
    int sock;
    uint8_t request_data[256];
    struct coap_option options[4];
    struct iovec iov;
    struct msghdr message;

    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    // Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server.sin_addr);

    // Initialize CoAP packet
    coap_packet_init(&request, request_data, sizeof(request_data), 
                     COAP_VERSION_1, COAP_TYPE_CON,
                     0, coap_next_token(), COAP_METHOD_POST, coap_next_id());

    // Set CoAP request options
    coap_option_add(&request, COAP_OPTION_URI_PATH, 
                    COAP_PATH, strlen(COAP_PATH));

    // Set CoAP request payload
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)COAP_MSG_CONTENT, 
                               strlen(COAP_MSG_CONTENT));

    // Configure message for sending
    iov.iov_base = request_data;
    iov.iov_len = request.offset;
    message.msg_name = &server;
    message.msg_namelen = sizeof(server);
    message.msg_iov = &iov;
    message.msg_iovlen = 1;

    // Send the CoAP message
    if (sendmsg(sock, &message, 0) < 0) {
        printk("Failed to send CoAP message\n");
    } else {
        printk("CoAP message sent successfully\n");
    }

    // Close the socket
    close(sock);
}