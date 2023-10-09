#include "common.h"

int main(int argc, char **argv){
    char* protocol = argv[1];
    char* gate = argv[2];
    
    struct sockaddr_storage storage;
    if(addrparse(protocol, gate, &storage) != 0){
        logexit("addrparse");
    }

    //Socket
    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    if(s == -1){
        logexit("socket");
    }

    //Conexao
    struct sockaddr *addr = (struct sockaddr *)(&storage); 
    if(connect(s, addr, sizeof(storage)) != 0){
        logexit("connect");
    }

    char input[1024];
    bool gameStarted = false;
    struct action req;

    while(scanf("%s", input) != EOF){
        //verifica se o comando é valido
        bool _send = true;
        if(strcmp(input, "start") == 0){
            req.type = 0;
            gameStarted = true;
        }
        else if(strcmp(input, "reset") == 0){
                req.type = 5;
                gameStarted = true;
            }
        else if(strcmp(input, "exit") == 0){
            req.type = 7;
        }
        else if(gameStarted){
            if(strcmp(input, "reveal") == 0){
                req.type = 1;
                scanf("%d,%d", &req.coordinates[0], &req.coordinates[1]);
                //verifica se a celula é valida
                if((req.coordinates[0] > 3 || req.coordinates[0] < 0)  || (req.coordinates[1] > 3 || req.coordinates[1] < 0)){
                    printf("error: invalid cell\n");
                    _send = false;
                    continue;
                }
                //verifica se a celula ja foi revelada
                if(req.board[req.coordinates[0]][req.coordinates[1]] != -2 && req.board[req.coordinates[0]][req.coordinates[1]] != -3){
                    printf("error: cell already revealed\n");
                    _send = false;
                    continue;
                }
            }
            else if(strcmp(input, "flag") == 0){
                req.type = 2;
                scanf("%d,%d", &req.coordinates[0], &req.coordinates[1]);
                //verifica se a celula é valida
                if((req.coordinates[0] > 3 || req.coordinates[0] < 0)  || (req.coordinates[1] > 3 || req.coordinates[1] < 0)){
                    printf("error: invalid cell\n");
                    _send = false;
                    continue;
                }
                //verifica se a celula ja tem flag
                if(req.board[req.coordinates[0]][req.coordinates[1]] == -3){
                    printf("error: cell already has a flag\n");
                    _send = false;
                    continue;
                }
                //verifica se a celula ja foi revelada
                if(req.board[req.coordinates[0]][req.coordinates[1]] >= 0){
                    printf("error: cannot insert flag in revealed cell\n");
                    _send = false;
                    continue;
                }
            }
            else if(strcmp(input, "remove_flag") == 0){
                req.type = 4;
                scanf("%d,%d", &req.coordinates[0], &req.coordinates[1]);
                //verifica se a celula é valida
                if((req.coordinates[0] > 3 || req.coordinates[0] < 0)  || (req.coordinates[1] > 3 || req.coordinates[1] < 0)){
                    printf("error: invalid cell\n");
                    _send = false;
                    continue;
                }
                //verifica se a celula tem flag
                if(req.board[req.coordinates[0]][req.coordinates[1]] != -3){
                    printf("error: cannot remove flag from a unflagged cell\n");
                    _send = false;
                    continue;
                }
            }
            else{
                printf("error: command not found\n");
                _send = false;
                continue;
            }
        }
        else{
            printf("error: game not started\n");
            _send = false;
            continue;
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
                req = resp;
                continue;
            }
            else if(resp.type == 6){
                printf("YOU WIN\n");
                gameStarted = false;
                print_matrix(resp.board);
                req = resp;
                continue;
            }
            else if(resp.type == 7){ 
                req = resp;
                exit(1);
            }
            //printa a matriz atualizada
            print_matrix(resp.board);
            //atualiza a matriz do cliente
            req = resp;
        }
    }
}