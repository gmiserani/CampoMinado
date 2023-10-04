#include "common.h"

int intitial_matrix[4][4];

//função que inicia a matriz
struct action start_matrix(struct action action){
    for(int i=0; i < 4; i++){
        for(int j=0; j < 4; j++){
            action.board[i][j] = -2;
            //printf("%d\t\t",action->board[i][j]);
        }
    }
    action.coordinates[0]= 0;
    action.coordinates[1]= 0;
    return action;
}

//função que verifica se o jogador ganhou comparando o board dele com o board resposta
bool win(struct action action){
    for(int i=0; i < 4; i++){
        for(int j=0; j < 4; j++){
            if(!(action.board[i][j] == intitial_matrix[i][j])){
                //se for bomba vai ser diferente
                if(intitial_matrix[i][j] != -1){
                    return false;
                }
            }
        }
    }
    return true;
}


int main(int argc, char *argv[]) {
    char* protocol = argv[1];
    char* gate = argv[2];
    if(strcmp(argv[3], "-i") == 0){
        FILE *fp;
        fp = fopen(argv[4], "r");
        char comma;        

        for(int i=0; i < 4; i++){
            for(int j=0; j < 4; j++){
                fscanf(fp, "%d%c", &intitial_matrix[i][j], &comma);
            }
        }
        fclose(fp);
        print_matrix(intitial_matrix);
    }
    else{
        exit(1);
    }

    struct sockaddr_storage storage;

    if(server_sockaddr_init(protocol, gate, &storage)){
        logexit("server_sockaddr_init");
    }

    // Socket
    int s = socket(storage.ss_family, SOCK_STREAM, 0);
    if(s == -1){
        logexit("socket");
    }

    // Reuse
    int enable = 1;
    if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) != 0){
        logexit("setsockopt");
    }

    // Bind
    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if(bind(s, addr, sizeof(storage)) != 0){
        logexit("bind");
    }

    // Listen
    if(listen(s, 10) != 0){
        logexit("listen");
    }
    struct sockaddr_storage cstorage;
    struct sockaddr *caddr = (struct sockaddr *) &cstorage;
    socklen_t caddrlen = sizeof(cstorage);

    //while client is connected
    while(1){
        int csock = accept(s, caddr, &caddrlen);
        printf("client connected\n");
        if (csock == -1) {
            logexit("accept");
        }
        while (1){
            struct action req;
            memset(&req, 0, sizeof(req));
            ssize_t count = recv(csock, &req, sizeof(req), 0);
            if (count == 0) {
                break;
            }
            else if (count != sizeof(req)) {
                logexit("recv");
            }

            struct action resp;
            //start
            if(req.type == 0){
                resp = start_matrix(req);
            }
            //reveal
            else if(req.type == 1){
                resp.board[req.coordinates[0]][req.coordinates[1]] = intitial_matrix[req.coordinates[0]][req.coordinates[1]];
                //bomb
                if(intitial_matrix[req.coordinates[0]][req.coordinates[1]] == -1){
                    resp.board[req.coordinates[0]][req.coordinates[1]] = -1;
                    resp.type = 8;
                }
                //ganhou
                else if(win(resp))
                    resp.type = 6;
                //state
                else
                    resp.type = 3;
            }
            //flag
            else if(req.type == 2){
                resp.board[req.coordinates[0]][req.coordinates[1]] = -3;
                resp.type = 3;
            }
            //remove flag
            else if(req.type == 4){
                resp.board[req.coordinates[0]][req.coordinates[1]] = -2;
                resp.type = 3;
            }
            //reset
            else if(req.type == 5){
                start_matrix(req);
                printf("starting new game");
                resp.type = 3;
            }
            else if(req.type == 7){
                start_matrix(req);
                printf("client disconnected\n");
                continue;
            }

            //verifica oq é pra fazer com a response
       
            if (send(csock, &resp, sizeof(resp), 0) != sizeof(resp)) {
                logexit("send");
            }
        }
        close(csock);
    }
    return 0;
}