#include <zephyr/zephyr.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socket.h>
#include <zephyr/sys/printk.h>

int main() {
    struct sockaddr_in server_addr;
    int sock;
    char *server_ip = "47.102.103.1";
    int server_port = 5683;
    
    char *uri_path = "/coap/test";
    char payload[] = "Hello, COAP Cloud";
    struct coap_packet request;
    uint8_t *data;
    size_t data_len = 256; // Buffer size

    data = (uint8_t *) malloc(data_len);
    if (!data) {
        printk("Failed to allocate memory for CoAP request\n");
        return -1;
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket: %d\n", errno);
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    coap_packet_init(&request, data, data_len, 1, 0, COAP_TYPE_CON, 
                     COAP_METHOD_POST, coap_next_token(), zoap_next_id());

    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, 
                              uri_path, strlen(uri_path));

    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, 
                               (uint8_t *)payload, strlen(payload));

    if (sendto(sock, request.data, request.offset, 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printk("Failed to send CoAP request: %d\n", errno);
    } else {
        printk("CoAP request sent successfully\n");
    }

    close(sock);
    free(data);

    return 0;
}