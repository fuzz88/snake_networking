#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9000

#include "drawing.h"
#include "game.h"


typedef struct _DataPacket {
    uint16_t    player_id;
    Point       data; 
} DataPacket;


void serialize(const DataPacket *data, char *buffer) {
    memcpy(buffer, data, sizeof(DataPacket));
}

void deserialize(const char *buffer, DataPacket *data) {
    memcpy(data, buffer, sizeof(DataPacket));
}

static void *client_func(void *data) {
    Game *game = (Game*)data;
    int sock;
    struct sockaddr_in server_addr;
    char buffer[sizeof(DataPacket)];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return NULL;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return NULL;
    }
    Point p = {.x = 69, .y = 13};
    DataPacket send_data = { .player_id = game->players[0]->id,  .data = p};
    DataPacket recv_data; 

    while (true) {
        serialize(&send_data, buffer);
        send(sock, buffer, sizeof(DataPacket), 0);
        printf("%u sent\n", send_data.player_id);

        recv(sock, buffer, sizeof(DataPacket), 0);
        deserialize(buffer, &recv_data);

        printf("%u connected\n", recv_data.player_id);
        sleep(1);
    }
    return NULL;
}

int main(void)
{
    Game *game = init_game();

    printf("my player id is %u\n", game->players[0]->id);

    pthread_t tcp_client_thread;
    if (pthread_create(&tcp_client_thread, NULL, client_func, game) != 0) {
        printf("ERROR: thread create failed");
    } 
    //pthread_detach(thread); 

    drawing_loop(game);
    return 0;
}
