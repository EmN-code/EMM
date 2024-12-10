
#include <zephyr/kernel.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socket.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static int send_coap_request(const char *server_ip, uint16_t server_port, const char *uri_path, const char *payload)
{
    int sock;
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t buffer[128];
    int ret;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket: %d\n", sock);
        return sock;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    ret = inet_pton(AF_INET, server_ip, &server_addr.sin_addr);
    if (ret < 0) {
        printk("Invalid address: %d\n", ret);
        close(sock);
        return ret;
    }

    ret = coap_packet_init(&request, buffer, sizeof(buffer), COAP_VERSION_1, COAP_TYPE_CON, 8,
                           coap_next_token(), COAP_METHOD_POST, coap_next_id());
    if (ret < 0) {
        printk("Failed to initialize CoAP packet: %d\n", ret);
        close(sock);
        return ret;
    }

    ret = coap_packet_append_option(&request, COAP_OPTION_URI_PATH, uri_path, strlen(uri_path));
    if (ret < 0) {
        printk("Failed to append URI path option: %d\n", ret);
        close(sock);
        return ret;
    }

    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        printk("Failed to append payload marker: %d\n", ret);
        close(sock);
        return ret;
    }

    ret = coap_packet_append_payload(&request, payload, strlen(payload));
    if (ret < 0) {
        printk("Failed to append payload: %d\n", ret);
        close(sock);
        return ret;
    }

    ret = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printk("Failed to send CoAP request: %d\n", ret);
        close(sock);
        return ret;
    }

    printk("CoAP request sent successfully\n");
    close(sock);
    return 0;
}

int main(void)
{
    printk("Starting CoAP client\n");

    const char *payload = "Hello, COAP Cloud";
    int ret = send_coap_request(SERVER_IP, SERVER_PORT, URI_PATH, payload);
    if (ret < 0) {
        printk("Failed to send CoAP request: %d\n", ret);
    }

    return 0;
}
