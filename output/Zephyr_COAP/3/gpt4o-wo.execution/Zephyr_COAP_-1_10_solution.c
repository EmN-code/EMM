#include zephyr/net/websocket.h
#include zephyr/net/net_ip.h
#include zephyr/net/net_mgmt.h
#include zephyr/net/dns_resolve.h
#include zephyr/device.h
#include zephyr/posix/sys/socket.h
#include zephyr/posix/unistd.h
#define COAP_BUF_SIZE 256

void send_coap_request(void)
{
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5683);
    inet_pton(AF_INET, "47.102.103.1", &server_addr.sin_addr);

    uint8_t buffer[COAP_BUF_SIZE];
    struct coap_packet request;

    coap_packet_init(&request, buffer, sizeof(buffer), 1, COAP_TYPE_CON, 0, NULL, COAP_METHOD_POST, coap_next_id());
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, (uint8_t *)"coap/test", strlen("coap/test"));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    send(sock, buffer, coap_packet_get_len(&request), 0);

    close(sock);
}

int main(void)
{
    while (1) {
        send_coap_request();
        sleep(10);
    }
    return 0;
}
