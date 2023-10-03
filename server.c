#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct action{
    int type;
    int coordinates[2];
    int board[4][4];
};
int intitial_matrix[4][4];

//print da matriz inicial
void print_matrix(int matrix[4][4]){
    for(int i=0; i < 4; i++){
        for(int j=0; j < 4; j++){
            if(matrix[i][j]== -1)
                printf("*\t\t");
            else if (matrix[i][j]== -2)
            {
                printf("-\t\t");
            }
            else{
                printf("%d\t\t", matrix[i][j]);
            }
        }
        printf("\n");
    }
}

//função que inicia a matriz
struct action start_matrix(struct action action){
    for(int i=0; i < 4; i++){
        for(int j=0; j < 4; j++){
            action.board[i][j] = -2;
            //printf("%d\t\t",action->board[i][j]);
        }
    }
    return action;
}

//função que atualiza a matriz
struct action update_matrix(struct action action){
    int x = action.coordinates[0];
    int y = action.coordinates[1];
    if(action.type == 0){
        action = start_matrix(action);
        print_matrix(action.board);
        return action;
    }
    else if(action.type == 1){
        if(intitial_matrix[x][y] == -1){
            printf("GAME OVER!");
            print_matrix(intitial_matrix);
            return action;
        }
        else if(intitial_matrix[x][y] == -2){
            action.board[x][y] = -2;
            print_matrix(action.board);
            return action;
        }
        else{
            action.board[x][y] = intitial_matrix[x][y];
            print_matrix(action.board);
            return action;
        }
    }
    else if(action.type == 2){
        action.board[x][y] = -3;
        print_matrix(action.board);
        return action;
    }
    // else if(action.type == 3){
    //     action.board[x][y] = -2;
    //     print_matrix(action.board);
    //     return action;
    // }
    else if(action.type == 4){
        action.board[x][y] = -2;
        print_matrix(action.board);
        return action;
    }
    else if(action.type == 5){
        restart_game();
        return action;
    }
    else if(action.type == 6){
        action.board[x][y] = -5;
        print_matrix(action.board);
        return action;
    }
    else if(action.type == 7){
        printf("client disconnected");
        exit;
    }
    else if(action.type == 8){
        printf("GAME OVER!");
        return action;
    }
    else{
        printf("error: command not found\n");
        return action;
    }   
    
}



int main(int argc, char *argv[]) {
    char* protocol = argv[1];
    char* gate = argv[2];
    // printf("%s\n", protocol);
    // printf("%s\n", gate);

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
    printf("\n");

    struct action action;
    action.type = 0;
    action.coordinates[0] = 0;
    action.coordinates[1] = 0;
    //start_matrix(&action);
    action = update_matrix(action);


    
    //    printf("%d %d\n", action.coordinates[1], action.coordinates[2]);
    // for(int i=0; i < 4; i++){
    //     for(int j=0; j < 4; j++){
    //         printf("%d", action.board[i][j]);
    //     }
    //     printf("\n");
    // }
    return 0;
}