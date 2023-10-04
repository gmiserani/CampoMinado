#include "common.h"

int main(int argc, char **argv){
    char* protocol = argv[1];
    char* gate = argv[2];
    
    struct sockaddr_storage storage;
    if(addrparse(protocol, gate, &storage) != 0){
        logexit("addrparse");
    }

    // Inicializar Socket
    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    if(s == -1){
        logexit("socket");
    }

    // Inicializar Conexao
    struct sockaddr *addr = (struct sockaddr *)(&storage); 
    if(connect(s, addr, sizeof(storage)) != 0){
        logexit("connect");
    }

    char input[1024];
    bool gameStarted = false;
    bool _send = true;
    struct action req;

    while(scanf("%s", input) != EOF){
        if(strcmp(input, "start") == 0){
            req.type = 0;
            printf("game started\n");
            gameStarted = true;
        }
        else if(gameStarted){
            if(strcmp(input, "reveal") == 0){
            req.type = 1;
            scanf("%d,%d", &req.coordinates[0], &req.coordinates[1]);
            if((req.coordinates[0] > 3 || req.coordinates[0] < 0)  || (req.coordinates[1] > 3 || req.coordinates[1] < 0)){
                printf("error: invalid cell\n");
                _send = false;
            }
            if(req.board[req.coordinates[0]][req.coordinates[1]] != -2){
                printf("error: cell already revealed");
                _send = false;
            }
            }
            else if(strcmp(input, "flag") == 0){
                req.type = 2;
                scanf("%d,%d", &req.coordinates[0], &req.coordinates[1]);
                if((req.coordinates[0] > 3 || req.coordinates[0] < 0)  || (req.coordinates[1] > 3 || req.coordinates[1] < 0)){
                    printf("error: invalid cell\n");
                    _send = false;
                }
                if(req.board[req.coordinates[0]][req.coordinates[1]] == -3){
                    printf("error: cell already has a flag");
                    _send = false;
                }
                if(req.board[req.coordinates[0]][req.coordinates[1]] >= 0){
                    printf("error: cannot insert flag in revealed cell");
                    _send = false;
                }
            }
            else if(strcmp(input, "remove_flag") == 0){
                req.type = 4;
                scanf("%d,%d", &req.coordinates[0], &req.coordinates[1]);
                if((req.coordinates[0] > 3 || req.coordinates[0] < 0)  || (req.coordinates[1] > 3 || req.coordinates[1] < 0)){
                    printf("error: invalid cell\n");
                    _send = false;
                }
                if(req.board[req.coordinates[0]][req.coordinates[1]] != -3){
                    printf("error: cannot remove flag from a unflagged cell");
                    _send = false;
                }
            }
            else if(strcmp(input, "reset") == 0){
                req.type = 5;
            }
            else if(strcmp(input, "exit") == 0){
                req.type = 7;
            }
            else{
                printf("error: command not found\n");
                _send = false;
            }
        }
        else{
            printf("error: game not started\n");
            _send = false;
        }

        if(_send){
            if(send(s, &req, sizeof(req), 0) == -1){
                logexit("send");
            }
            struct action resp;
            if(recv(s, &resp, sizeof(resp), 0) == -1){
                logexit("recv");
            }
            if(resp.type == 8){
                printf("GAME OVER\n");
                gameStarted = false;
                print_matrix(resp.board);
                continue;
            }
            else if(resp.type == 6){
                printf("YOU WIN\n");
                gameStarted = false;
                print_matrix(resp.board);
                continue;
            }
            print_matrix(resp.board);
            req = resp;
        }
        
    }

}