#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

/* Main function to send CoAP POST request */
int main(void) {
    int sock;
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t request_buffer[128];
    const char *uri_path = URI_PATH;
    const char *payload = PAYLOAD;

    /* Create a socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Error creating socket\n");
        return -1;
    }

    /* Configure server address */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    /* Initialize CoAP request */
    coap_packet_init(&request, request_buffer, sizeof(request_buffer), COAP_VERSION_1);
    coap_packet_append_uri_path(&request, uri_path);
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (const uint8_t *)payload, strlen(payload));

    /* Send CoAP request */
    if (sendto(sock, request_buffer, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printk("Error sending CoAP request\n");
        close(sock);
        return -1;
    }

    printk("CoAP request sent successfully\n");

    /* Close socket */
    close(sock);

    return 0;
}