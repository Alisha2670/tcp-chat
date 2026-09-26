#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024
#define USERNAME_LEN 32

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *server_ip = argv[1];
    int port = atoi(argv[2]);
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char username[USERNAME_LEN];

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid IP address: %s\n", server_ip);
        close(sock_fd);
        return EXIT_FAILURE;
    }

    printf("Connecting to %s:%d...\n", server_ip, port);

    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    printf("[+] Connected to server!\n");

    while (1) {
        printf("Enter your username: ");
        fflush(stdout);

        if (fgets(username, sizeof(username), stdin) == NULL) {
            close(sock_fd);
            return EXIT_FAILURE;
        }

        username[strcspn(username, "\n")] = '\0';

        if (strlen(username) > 0) {
            break;
        }
        printf("Username cannot be empty. Please try again.\n");
    }

    if (send(sock_fd, username, strlen(username), 0) < 0) {
        perror("send username");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    printf("[+] Registered as '%s'. Type messages below (type 'exit' to quit):\n", username);

    while (1) {
        printf("> ");
        fflush(stdout);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (strlen(buffer) == 0) {
            continue;
        }

        if (strcmp(buffer, "exit") == 0) {
            printf("[*] Exiting chat...\n");
            break;
        }

        if (send(sock_fd, buffer, strlen(buffer), 0) < 0) {
            perror("send");
            break;
        }
    }

    close(sock_fd);
    printf("[*] Disconnected.\n");

    return EXIT_SUCCESS;
}