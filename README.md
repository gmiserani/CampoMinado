# CampoMinado
trabalho prático da disciplina de redes

Trabalho Prático 1 - Servidor de Campo Minado
Nome: Gabriela Moraes Miserani de Freitas
Matrícula: 2020006566
Introdução
O trabalho prático da disciplina de Redes de Computadores consiste no desenvolvimento do jogo Campo Minado, na linguagem C, seguindo as regras originais do jogo, em um ambiente de interação entre um Servidor e um Cliente. Essa interação, por sua vez, foi feita usando Sockets (interface POSIX de sockets de redes) no sistema Linux. Para tal, os papéis do Servidor e do Cliente foram muito bem definidos, de tal forma que foi possível definir com clareza quais as mensagens  que deveriam ser trocadas entre eles. O formato dessa mensagem, por sua vez, foi definido a partir de uma estrutura:
struct action {
  int type;
  int coordinates[2];
  int board[4][4];
};

Onde 'type' representa a jogada, 'coordinates' a posição do campo onde a jogada será realizada e 'board' o campo atual no que tange a jogada realizada.
Servidor
O arquivo inicial a ser executado, este programa implementa um servidor para um jogo de campo minado. Ele é responsável por receber comandos do cliente, como inicializar a partida, revelar células e colocar bandeiras, verificar se o jogador ganhou ou perdeu o jogo, etc., e atualiza o tabuleiro de acordo. A partir disso, devolve para o cliente uma resposta contendo o status e o tabuleiro atualizado. Por fim, há, no arquivo do servidor, o estabelecimento da conexão com o cliente por meio de Sockets.
O funcionamento do servidor segue o seguinte fluxo:
1. Inicialização do servidor.
   - Criação do socket
   - Configuração do socket
   - Vinculação do socket a um endereço e porta.
   - Espera por conexões de clientes.
2. Loop para aceitar clientes.
   - Aceita uma conexão de cliente.
3. Loop para lidar com comandos do cliente.
   - Recebe comandos do cliente.
   - Processa os comandos e atualiza o estado do jogo.
   - Envia respostas ao cliente.
Cliente
O programa começa obtendo argumentos da linha de comando, esperando receber o IP e o número da porta para se conectar ao servidor. Em seguida, o programa chama a função `addrparse` para converter o endereço de IP e o número da porta em uma estrutura de endereço (`struct sockaddr_storage`). Se a conversão falhar, o programa exibe uma mensagem de erro e sai. A partir disso um socket é criado usando as informações do endereço obtidas na etapa anterior. O programa tenta se conectar ao servidor e, então, entra em um loop principal que aguarda a entrada do usuário. Ele lê comandos do usuário, verifica-os e realiza o comando. Se o comando for válido, o programa envia a estrutura de ação (`req`) para o servidor usando a função `send`. Em seguida, ele espera uma resposta do servidor usando a função `recv`. A matriz do jogo é atualizada com os dados recebidos do servidor.O loop continua até que o usuário insira o comando "exit" ou até que ocorra algum erro de comunicação ou conexão.
Common
Nesse arquivo, é declarada a  `struct action`, que representa uma ação do jogador no jogo. Além disso, nele são inseridas as funções de uso comum entre o servidor e o cliente, sendo elas: print_matrix, que imprime a matriz do jogo; e as funções server_sockaddr_init, addrparse e addrtostr, responsáveis por gerenciar o socket e a comunicação entre o cliente e servidor. Por fim, também há a função logexit para gerir registro de mensagens em caso de falha.
Desafios encontrados
Ao desenvolver o TP, o principal desafio encontrado foi estabelecer e gerir a conexão entre cliente e servidor. No uso inicial de "sockets", foi preciso estudo do funcionamento do envio e recebimento de mensagens a partir do send e recv, para, assim, entender como e qual o formato da mensagem a ser enviada e uma vez recebida, como acessar os dados. Para entender melhor foi preciso, além do material disponibilizado, procurar no YouTube exemplos diversos para entender melhor esse fluxo.
Uma vez compreendido o funcionamento da troca de mensagens, foi encontrada certa dificuldade no entendimento completo dos papéis do cliente e servidor, principalmente no que tange o que cada um deveria enviar/receber e quais alterações cada um tem a responsabilidade de fazer. Para tal, foi preciso rever os exemplos disponíveis na especificação do trabalho, além do estudo minucioso da seguinte tabela fornecida pelo professor: 


A partir da tabela acima foi possível entender corretamente o papel do servidor e do cliente e, portanto, inferir quais funções deveriam estar em qual arquivo.
	Uma outra dificuldade encontrada foi em relação à forma de lidar com os erros. Isso porque esse item envolve não apenas que os erros sejam impressos no terminal, mas que primeiramente sejam identificados e que, uma vez feito, o jogo continue esperando um novo comando. Para tal, foi preciso analisar exatamente quais situações poderiam ocasionar erros e verificar a existência de tais situações para garantir que os erros sejam dados corretamente. Uma vez feita essas verificações, ainda precisa-se garantir que o fluxo do jogo não seria interrompido, ou seja, que uma vez que o erro seja apresentado, o jogador possa continuar jogando. Foi verificado que, para que isso acontecesse, uma vez que estivermos dentro do if que verifica o erro, deveríamos voltar ao início do loop principal de forma a ler a próxima entrada. Para compreender isso foi preciso analisar melhor o fluxo do projeto e onde seria esperado ir uma vez que o erro fosse encontrado. Portanto, para manter o fluxo como esperado, foi preciso apenas inserir o comando continue para cada situação em que um erro foi encontrado.


Por fim, um outro desafio encontrado foi o de verificar a vitória. Isso se deve principalmente porque, inicialmente, estava sendo comparado apenas o board do cliente com o da resposta armazenada pelo servidor. Ao testar, porém, foi percebido que não estava declarando vitória corretamente, e que isso se devia, principalmente, ao fato de que a matriz final do cliente não é exatamente igual à matriz resposta, uma vez que deveria levar em conta que as posições onde as bombas estão localizadas permaneceram ocultas ou com flags. Dessa forma, foi preciso levar essas possibilidades em consideração ao fazer a verificação.

Conclusão
A partir do desenvolvimento deste trabalho, foi possível entender melhor a comunicação em rede entre um servidor e um cliente. Partindo do estudo do funcionamento dos Sockets, foi possível entender o que é necessário para manter uma comunicação segura e como manter um formato de mensagem tal que ambos servidor e cliente sejam capazes de entender e operar em cima. Além disso, uma vez concluído o trabalho, tornou-se claro a importância desse tipo de comunicação e as diferentes formas que pode ser empregada. Para finalizar, segue a instrução para compilar e executar o trabalho:

cliente: ./bin/client <IP> <porta>
servidor: ./bin/server <v4/v6> <porta> -i <path do arquivo de entrada dos dados>
