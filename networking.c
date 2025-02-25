#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "networking.h"
#include "defines.h"
#include "game.h"

void send_data(int sockfd, DataPacket *packet)
{
    // Send fixed-size header first
    send(sockfd, &packet->player_id, sizeof(packet->player_id), 0);
    send(sockfd, &packet->length, sizeof(packet->length), 0);

    // Send variable-length snake data
    if (packet->length > 0)
    {
        send(sockfd, packet->snake, packet->length * sizeof(SnakeSegment), 0);
    }
}

void receive_data(int sockfd, DataPacket *packet)
{
    // Receive fixed-size header first
    recv(sockfd, &packet->player_id, sizeof(packet->player_id), 0);
    recv(sockfd, &packet->length, sizeof(packet->length), 0);

    // Allocate memory for the variable-length snake array
    if (packet->length > 0)
    {
        packet->snake = malloc(packet->length * sizeof(SnakeSegment));
        recv(sockfd, packet->snake, packet->length * sizeof(SnakeSegment), 0);
    }
    else
    {
        packet->snake = NULL;
    }
}

void *client_func(void *data)
{
    Game *game = (Game *)data;

    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[sizeof(DataPacket)];

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("ERROR: socket creation failed");
        return NULL;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("ERROR: socket connection failed");
        close(sockfd);
        return NULL;
    }

    DataPacket packet;
    packet.player_id = game->players[0]->id;

    DataPacket received;

    while (true)
    {
        packet.length = game->world->snakes[0]->length;
        packet.snake = malloc(packet.length * sizeof(SnakeSegment));

        memcpy(packet.snake, game->world->snakes[0]->body, packet.length * sizeof(SnakeSegment));

        send_data(sockfd, &packet);
        sleep(1 / TARGET_FPS / SNAKE_MAX_SPEED);
        receive_data(sockfd, &received);

        bool new_player = true;
        for (size_t i = 0; i < game->players_count; ++i)
        {
            if (game->players[i]->id == received.player_id)
            {
                new_player = false;
                game->world->snakes[game->players[i]->snake_idx]->length = received.length;
                memcpy(game->world->snakes[game->players[i]->snake_idx]->body, received.snake, received.length * sizeof(SnakeSegment));
            }
        }
        if (new_player)
            add_player(game, &received);

        free(packet.snake);
        free(received.snake);
    }

    return NULL;
}

void start_networking(Game *game)
{
    pthread_t tcp_client_thread;
    if (pthread_create(&tcp_client_thread, NULL, client_func, game) != 0)
    {
        printf("ERROR: thread create failed");
    }
}