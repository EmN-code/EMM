#include <zephyr.h>
#include <net/coap.h>
#include <net/socket.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define MAX_COAP_MSG_LEN 256

void main(void) {
    struct sockaddr_in server_sock;
    struct coap_packet request;
    uint8_t coap_buffer[MAX_COAP_MSG_LEN];
    const char *payload = "Hello, COAP Cloud";
    int sock;

    // Create the socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    // Define server address
    server_sock.sin_family = AF_INET;
    server_sock.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_sock.sin_addr);

    // Initialize CoAP request
    coap_packet_init(&request, coap_buffer, sizeof(coap_buffer), 1,
                     COAP_TYPE_CON, 8, coap_next_token(),
                     COAP_METHOD_POST, coap_next_id());

    // Set URI path
    coap_append_option(&request, COAP_OPTION_URI_PATH, "coap/test", strlen("coap/test"));
    
    // Set payload
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)payload, strlen(payload));

    // Send the CoAP request
    if (sendto(sock, request.data, request.offset, 0,
               (struct sockaddr *)&server_sock, sizeof(server_sock)) < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return;
    }

    printk("CoAP POST request sent to %s:%d\n", SERVER_ADDR, SERVER_PORT);

    // Close the socket
    close(sock);
}
