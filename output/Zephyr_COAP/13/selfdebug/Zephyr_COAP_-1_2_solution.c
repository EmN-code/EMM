
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socketutils.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

int main(void)
{
    int sock;
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t buffer[128];
    int ret;

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Initialize the CoAP request
    ret = coap_packet_init(&request, buffer, sizeof(buffer), COAP_TYPE_CON, COAP_METHOD_POST, 0, NULL, 0, URI_PATH, strlen(URI_PATH));
    if (ret < 0) {
        printk("Failed to initialize CoAP packet\n");
        close(sock);
        return -1;
    }

    // Add payload to the CoAP request
    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        printk("Failed to append payload marker\n");
        close(sock);
        return -1;
    }

    ret = coap_packet_append_payload(&request, MESSAGE, strlen(MESSAGE));
    if (ret < 0) {
        printk("Failed to append payload\n");
        close(sock);
        return -1;
    }

    // Send the CoAP request
    ret = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    printk("CoAP request sent successfully\n");

    // Close the socket
    close(sock);

    return 0;
}
