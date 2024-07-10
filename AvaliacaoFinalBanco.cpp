/*Criar um sistema de atendimento e controle de senhas para um banco.

Exigências (3,0)

Utilizar estrutura de dados na confecção do código.
Lista, fila ou pilha 
Recursividade
Busca e ordenação

OBS:. O uso de todas as exigências não caracteriza pontuação máxima. Haverá analise de criteriosa sobre semântica da estrutura apresentada.

Menu (1,0) 

Gerar Senha - [Senha específica para tipo de operação]
Atendimento 
Controle de senha
Exclusão de senha

Serviços  (6,0)

Abertura/Encerramento  de conta
Transferências
Saldo

OBS:. Tratar todos os erros e exceções que podem ser cometidas pelo usuário*/

//incluindo bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

//definica de constantes de senha
#define TAM_CHAMADA_SENHA 5
#define CODIGO_PRIORIDADE "PR"
#define CODIGO_CONTA "GC"
#define CODIGO_TRANSF "GT"
#define CODIGO_SALDO "CS"
#define CODIGO_DEPOSITO "DE"
#define CODIGO_SAQUE "SA"
//definicao de constantes de tamanho de vetor
#define TAM_TITULAR 100
#define TAM_ID 9
#define TAM_NOME_ARQUIVO 18

//funcoes basicas
void limpabuffer(){
    fflush(stdin);
}
void pause(){
    system("pause");
}
void limpatela(){
    system("clear || cls");
}

//declaracao de struct das senhas
typedef struct No{
    char senha[TAM_CHAMADA_SENHA];
    struct No* prox;
} No;

//declaracao de struct das contas
struct TipoConta{      
    char titular[TAM_TITULAR];
    char id[TAM_ID];
    int senha;
    double saldo;
};
typedef struct TipoConta Conta;
Conta conta;

//declaracao de contadores 
int cont_pr = 0;
int cont_gc = 0;
int cont_gt = 0;
int cont_cs = 0;
int cont_de = 0;
int cont_sa = 0;
int cont_total = 0;

//declaracao de variaveis de administracao
char user_adm[] = "root";
char senha_adm[] = "senha";

//menu principal
int menu(){
    int op;
    
        limpatela();
        printf("======BEM VINDO AO JUQUINHA'S BANK======\n\n");
        printf("\tSelecione a opcao desejada: \n\n");
        printf("1.Gerar senha\n");
        printf("2.Efetuar atendimento\n");
        printf("3.Controle de senha\n");
        printf("\nFUNCOES RESTRITAS:\n\n");
        printf("4.Exclusao de senha\n");
        printf("5.Controle de contas\n");
        printf("----------------\n");
        printf("6.Sair\n");
        printf("\n> ");
        scanf("%d", &op);
        limpabuffer();
        limpatela();

    return op;
}

//menu de escolha de serviço
int menu_servico(){
    int op;

    printf("Qual tipo de servico deseja realizar?\n");
    printf("1. Abertura/Fechamento de conta\n");
    printf("2. Transferencias\n");
    printf("3. Consultar saldo\n");
    printf("4. Deposito\n");
    printf("5. Saque\n>");
    scanf("%d", &op);
    limpabuffer();

    return op;
}

//funcao para consultar prioridade ao gerar senha
int consulta_prioridade(){
    int aux;
    do{
        printf("E prioridade?\n1. SIM\n2.NAO\n>");
        scanf("%d", &aux);
        limpabuffer();
        if(aux != 1 && aux != 2){
            printf("Insira uma opcao valida!\n");
        }
    }while(aux != 1 && aux != 2);
    
    if(aux == 1) return 1;
    else return 0;  
}

//função para inserir no fim da lista
No* insere_fim(No* ini, char code[]) {
    No* novo = (No*)malloc(sizeof(No)); // Aloca espaço para o novo nó
    if (novo == NULL) { // Verifica se a alocação foi bem-sucedida
        printf("Erro de alocacao de memoria.\n");
        exit(1);
    }
    strcpy(novo->senha, code); // Copia o código da senha para o novo nó
    novo->prox = NULL; // Define o próximo nó como NULL

    if (ini == NULL) { // Caso a lista esteja vazia
        // Lista vazia, novo nó é o primeiro nó
        return novo;
    } else {
        // Percorre até o fim da lista e insere o novo nó
        No* p = ini;
        while (p->prox != NULL) { // Percorre a lista até o último nó
            p = p->prox;
        }
        p->prox = novo; // Define o próximo do último nó como o novo nó
        return ini;
    }
}

//funcao para inserir no inicio da lista
No* insere_inicio(No* ini, char code[]) {
    No* novo = (No*)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro de alocacao de memoria.\n");
        exit(1);
    }
    strcpy(novo->senha, code);
    novo->prox = ini; // O próximo nó será o antigo início da lista
    return novo;
}

//funcao para gerar senhas
void gerar_senha(No** senhas, int prioridade) {
    char codigo[TAM_CHAMADA_SENHA + 1];
    //caso seja prioridade
    if (prioridade) {
        cont_pr++;//incrementando contador de senhas prioritarias
        snprintf(codigo, TAM_CHAMADA_SENHA + 1, "%s%03d", CODIGO_PRIORIDADE, cont_pr); //o +1 no segundo parametro refere-se ao '\0' que é considerado um caracter pela funçao snprintf() 

        No* novo = (No*)malloc(sizeof(No));
        if (novo == NULL) {
            printf("Erro de alocação de memoria.\n");
            exit(1);
        }
        strcpy(novo->senha, codigo);
        novo->prox = NULL;

        if (*senhas == NULL || strncmp((*senhas)->senha, CODIGO_PRIORIDADE, strlen(CODIGO_PRIORIDADE)) != 0) {
            // Lista vazia ou a primeira senha não é prioridade -> coloca a senha de prioridade no topo da lista
            novo->prox = *senhas;
            *senhas = novo;
        } else {
            // Encontrar a última senha de prioridade -> se ja houver senhas prioritarias, a nova senha de prioridade sera inserida ao final dessas e antes das demais
            No* p = *senhas;
            while (p->prox != NULL && strncmp(p->prox->senha, CODIGO_PRIORIDADE, strlen(CODIGO_PRIORIDADE)) == 0) {
                p = p->prox;
            }
            novo->prox = p->prox;
            p->prox = novo;
        }
    } else {//caso nao seja prioritaria
        int op_serv;
        do {
            op_serv = menu_servico();//lendo do usuario o serviço que deseja gerar senha

            if (op_serv < 1 || op_serv > 5) {
                printf("Por favor, insira uma opcao valida\n");
            }
        } while (op_serv < 1 || op_serv > 5);//verificando se a entrada do usuario esta correta
        //tratando a senha de acordo com o serviço selecionado
        switch (op_serv) {
            case 1:
                cont_gc++;
                snprintf(codigo, TAM_CHAMADA_SENHA + 1, "%s%03d", CODIGO_CONTA, cont_gc);
                break;
            case 2:
                cont_gt++;
                snprintf(codigo, TAM_CHAMADA_SENHA + 1, "%s%03d", CODIGO_TRANSF, cont_gt);
                break;
            case 3:
                cont_cs++;
                snprintf(codigo, TAM_CHAMADA_SENHA + 1, "%s%03d", CODIGO_SALDO, cont_cs);
                break;
            case 4:
                cont_de++;
                snprintf(codigo, TAM_CHAMADA_SENHA + 1, "%s%03d", CODIGO_DEPOSITO, cont_de);
                break;
            case 5:
                cont_sa++;
                snprintf(codigo, TAM_CHAMADA_SENHA + 1, "%s%03d", CODIGO_SAQUE, cont_sa);
                break;
        }
        *senhas = insere_fim(*senhas, codigo); // Inserir no fim
    }

    printf("Sua senha: %s\n", codigo);
    pause();
}
//funcao para listar todas as senhas geradas
void listar_senhas(No* ini) {
    // char teste_user[100];
    // char teste_senha[50];

    // printf("AREA RESTRITA APENAS PARA USUARIOS AUTORIZADOS!\n\n");
    // printf("Insira o usuario:\n>");
    // scanf("%s", teste_user);
    // limpabuffer();

    // if(strcmp(teste_user, user_adm) != 0) printf("Usuario incorreto!\n");
    // else{
    //     printf("Insira a senha:\n>");
    //     limpabuffer();
    //     scanf("%s", teste_senha);
    //     limpabuffer();
    //     if(strcmp(teste_senha, senha_adm) != 0) printf("Senha incorreta!\n");
    //     else{
            if (ini == NULL) {
                printf("Nenhuma senha gerada.\n");
            } else {
                No* p = ini;
                printf("Senhas geradas:\n");
                while (p != NULL) {
                    printf("%s\n", p->senha);
                    p = p->prox;
                }
            }
        //}
    //}
    pause();
}

//funcao para excluir todas as senhas geradas
No* excluir_senha(No* ini, char code[], int type) {
    //conferindo se o usuario é autorizado para realizar a operacao
    if(type == 1){
        char teste_user[100];
        char teste_senha[50];
        
        printf("AREA RESTRITA APENAS PARA USUARIOS AUTORIZADOS!\n\n");
        printf("Insira o usuario:\n>");
        scanf("%s", teste_user);
        limpabuffer();

        if(strcmp(teste_user, user_adm) != 0){
            printf("Usuario incorreto!\n");
            pause();
            return ini;
        }
        else{
            printf("Insira a senha:\n>");
            limpabuffer();
            scanf("%s", teste_senha);
            limpabuffer();
            if(strcmp(teste_senha, senha_adm) != 0){
                printf("Senha incorreta!\n");
                pause();
                return ini;
            }
        }
    }
    //verificando se ha senhas na fila para serem excluidas
    if (ini == NULL) {
        printf("Nenhuma senha para excluir.\n");
        return NULL;
    }

    No* atual = ini;
    No* anterior = NULL;
    //percorrendo lista para verificar se a senha desejada existe
    while (atual != NULL && strcmp(atual->senha, code) != 0) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Senha %s nao encontrada.\n", code);//caso nao encontre a senha retorna mensagem de aviso
    } else {
        //exclui senha desejada da lista
        if (anterior == NULL) {
            ini = atual->prox; // Senha está no primeiro nó
        } else {
            anterior->prox = atual->prox; // Pular o nó a ser removido
        }
        free(atual);
        if(type == 1) printf("Senha %s excluida com sucesso.\n", code);        
    }      
    pause();
    return ini;
}
//verifica o codigo da senha para realizar o atendimento de acordo com o serviço
void verificaCodigo(char *senha, char *codigoServico) {
    strncpy(codigoServico, senha, 2);
    codigoServico[2] = '\0';
}

//funcao para colocar dados da conta dentro do arquivo
void geraConta(Conta conta){
    bool contaRepetida = false;
    int aux, digito;
    char nome_arq[TAM_NOME_ARQUIVO];
    FILE *fp;

    do{//gerando id aleatorio
        aux = rand() % 1000000;
        digito = rand() % 10;

        snprintf(conta.id, TAM_ID, "%06d-%d", aux, digito);
        snprintf(nome_arq, TAM_NOME_ARQUIVO, "conta%s.txt", conta.id);//gerando o nome do arquivo com base no id

        fp = fopen(nome_arq, "rb");
        if(fp == NULL){
            //criando e escrevendo no arquivo as informacoes do usuario
            fp = fopen(nome_arq, "wb+");
            fwrite(&conta, sizeof(conta), 1, fp);
            fclose(fp);
            //adicionando ao final do arquivo das contas, o id da nova conta
            fp = fopen("contas.txt", "a");
            //caso arquivo ainda nao exista, é criado
            if(fp == NULL){
                fp = fopen("contas.txt", "w+");
            }
            fprintf(fp, "%s\n", conta.id);
            contaRepetida = false;
        }
        else{//se != NULL -> já existe
            contaRepetida = true;
        }
        fclose(fp);

    }while(contaRepetida);//executando o loop enquanto a conta gerada for repetida e o id ja existir
    printf("O numero da sua conta e: %s\n", conta.id);
}
//funcao para abrir conta
void abrirConta(Conta conta){
    char auxiliar[200];
    int senhaLen;
    //solicitando informacoes ao usuario e fazendo verificacoes de quantidade maxima de digitos e de valor minimo de deposito
    do{
        printf("Insira o nome do titular com no maximo 100 digitos:\n>");
        limpabuffer();
        scanf("%[^\n]s", auxiliar);
        limpabuffer();
    }while(strlen(auxiliar) > TAM_TITULAR);
    strcpy(conta.titular, auxiliar);

    do{
        printf("Insira uma senha numerica de 6 digitos para a conta:\n>");
        scanf("%d", &conta.senha);
        limpabuffer();
        senhaLen = snprintf(NULL, 0, "%d", conta.senha);
        if(senhaLen != 6) 
            printf("Numero de digitos invalido, por favor, insira uma senha numerica de 6 digitos!\n");
    }while(senhaLen != 6);
    
    do{
        printf("Insira o valor do deposito inicial:\n>");
        scanf("%lf", &conta.saldo);
        limpabuffer();
        if(conta.saldo <= 0)
            printf("Nao e possivel criar a conta com um saldo inicial igual ou menor que 0! Por favor, insira um deposito inicial valido!\n");
    }while(conta.saldo <= 0);

    geraConta(conta);
    pause();

}
//funcao para encerrar uma conta
void encerrarConta(Conta conta){
    FILE *fp;
    char nome_arq[TAM_NOME_ARQUIVO];
    int teste_senha, senhaLen;
    char teste_id[TAM_ID];
    char auxiliar[20];

    // Obtendo os dados do usuário
    do {
        printf("Insira o numero da conta que deseja encerrar (xxxxxx-x)\n>");
        scanf("%s", auxiliar);
        limpabuffer();
    } while (strlen(auxiliar) > TAM_ID);
    strcpy(teste_id, auxiliar);

    snprintf(nome_arq, TAM_NOME_ARQUIVO, "conta%s.txt", teste_id);
    //abrindo arquivo para verificar se a conta existe antes de tentar excluir
    fp = fopen(nome_arq, "rb");
    if(fp == NULL){
        printf("Ocorreu um erro ao abrir o arquivo ou essa conta nao existe!\n");
    }
    else{
        //obtendo senha do usuario
        do{
            printf("Insira a senha numerica de 6 digitos da conta:\n>");
            scanf("%d", &teste_senha);
            limpabuffer();
            senhaLen = snprintf(NULL, 0, "%d", teste_senha);
            if(senhaLen != 6)  printf("Numero de digitos invalido, por favor, insira uma senha numerica de 6 digitos!\n");
        }while(senhaLen != 6);

        fread(&conta, sizeof(conta), 1, fp); //lendo do arquivo para comparar se a senha digitada pelo usuario é a mesma da conta

        if(teste_senha != conta.senha) printf("Senha incorreta, tente novamente!\n");//caso senhas nao coincidam
        
        else if(conta.saldo != 0){ //verificando se o saldo esta zerado para encerrar a conta
            printf("Para encerrar a conta o saldo da conta precisa ser zerado, por favor, retire ou transfira o saldo restante!\n");
            printf("Saldo restante: R$%.2lf\n", conta.saldo);
        }
        else{//apagando arquivo da conta
            fclose(fp);
            if(remove(nome_arq) == 0){
                printf("Conta encerrada com sucesso!\n");
            }
            else printf("Ocorreu um erro ao excluir o arquivo!\n");
        }
    }
    fclose(fp);
    pause();
}
//funcao para perguntar, ao atender senhas GC, se deseja abrir ou encerrar conta
void gerenciarConta(){
    int op;
    do{
        printf("1. Abrir conta\n2. Encerrar conta\n>");
        limpabuffer();
        scanf("%d", &op);
        limpabuffer();
        if(op != 1 && op != 2){
            printf("Por favor, insira uma opcao valida!\n");
            pause();
        }
    }while(op != 1 && op != 2);

    if(op == 1) abrirConta(conta);
    if(op == 2) encerrarConta(conta);
}
//funcao para transferencia
void transferencia(){
    FILE *fp;
    Conta origem, destino;
    char nome_arq_origem[TAM_NOME_ARQUIVO], nome_arq_destino[TAM_NOME_ARQUIVO], auxiliar[20];
    int senhaLen;

    // Obtendo os dados do usuário
    do {
        printf("Insira o numero da sua conta (xxxxxx-x):\n>");
        scanf("%s", auxiliar);
        limpabuffer();
    } while (strlen(auxiliar) > TAM_ID);
    strcpy(origem.id, auxiliar);

    snprintf(nome_arq_origem, TAM_NOME_ARQUIVO, "conta%s.txt", origem.id);

    fp = fopen(nome_arq_origem, "rb");
    if(fp == NULL){
        printf("Ocorreu um erro ao abrir o arquivo ou essa conta nao existe!\n");
    }
    else{
        do{
            printf("Insira a senha numerica de 6 digitos da conta:\n>");
            scanf("%d", &origem.senha);
            limpabuffer();
            senhaLen = snprintf(NULL, 0, "%d", origem.senha);
            if(senhaLen != 6)  printf("Numero de digitos invalido, por favor, insira uma senha numerica de 6 digitos!\n");
        }while(senhaLen != 6);

        fread(&conta, sizeof(conta), 1, fp);

        if(origem.senha != conta.senha) printf("Senha incorreta, tente novamente!\n");
        else{//caso informacoes de origem estejam corretas, validacao das informacoes de destino
            origem.saldo = conta.saldo;
            strcpy(origem.titular, conta.titular);
            do {
                printf("Insira o numero da conta para qual deseja transferir (xxxxxx-x):\n>");
                scanf("%s", auxiliar);
                limpabuffer();
            } while (strlen(auxiliar) > TAM_ID);
            strcpy(destino.id, auxiliar);

            snprintf(nome_arq_destino, TAM_NOME_ARQUIVO, "conta%s.txt", destino.id);

            fp = fopen(nome_arq_destino, "rb");
            if(fp == NULL){
                printf("Ocorreu um erro ao abrir o arquivo ou essa conta nao existe!\n");
            }
            else{//caso informacoes de origem e destino estejam corretas
                fread(&conta, sizeof(conta), 1, fp);
                destino.saldo = conta.saldo;
                destino.senha = conta.senha;
                strcpy(destino.titular, conta.titular);

                double valor;
                
                do{
                    printf("Insira o valor que deseja transferir:\n>");
                    scanf("%lf", &valor);
                    limpabuffer();
                }while(valor <= 0);
                //verificando se o saldo é suficiente para efetuar a operacao
                if(origem.saldo < valor) printf("Saldo insuficiente, por favor tente novamente!\n");
                else{//efetuando alteracao do saldo nas variaveis
                    origem.saldo -= valor;
                    destino.saldo += valor;
                    //efetuando alteracao do saldo no arquivo de origem
                    fp = fopen(nome_arq_origem, "w");
                    fwrite(&origem, sizeof(origem), 1, fp);
                    fclose(fp);
                    //efetuando alteracao do saldo no arquivo de destino
                    fp = fopen(nome_arq_destino, "w");
                    fwrite(&destino, sizeof(destino), 1, fp);
                    fclose(fp);

                    printf("Transferencia feita com sucesso!\n");
                    printf("Saldo final apos a transferencia: R$%.2f\n", origem.saldo);
                }
            }
        }
    }
    fclose(fp);
    pause();
}
//funcao para consultar saldo
void consultaSaldo(){
    FILE *fp;
    char nome_arq[TAM_NOME_ARQUIVO];
    int teste_senha, senhaLen;
    char teste_id[TAM_ID];
    char auxiliar[20];

    // Obtendo os dados do usuário
    do {
        printf("Insira o numero da conta que deseja consultar o saldo (xxxxxx-x)\n>");
        scanf("%s", auxiliar);
        limpabuffer();
    } while (strlen(auxiliar) > TAM_ID);
    strcpy(teste_id, auxiliar);

    snprintf(nome_arq, TAM_NOME_ARQUIVO, "conta%s.txt", teste_id);

    fp = fopen(nome_arq, "rb");
    if(fp == NULL){
        printf("Ocorreu um erro ao abrir o arquivo ou essa conta nao existe!\n");
    }
    else{
        do{
            printf("Insira a senha numerica de 6 digitos da conta:\n>");
            scanf("%d", &teste_senha);
            limpabuffer();
            senhaLen = snprintf(NULL, 0, "%d", teste_senha);
            if(senhaLen != 6)  printf("Numero de digitos invalido, por favor, insira uma senha numerica de 6 digitos!\n");
        }while(senhaLen != 6);
        //lendo informacoes do arquivo
        fread(&conta, sizeof(conta), 1, fp);
        //validando senha e retornando o saldo caso correta
        if(teste_senha != conta.senha) printf("Senha incorreta, tente novamente!\n");
        else printf("Saldo atual da conta: R$%.2lf\n", conta.saldo);
    }
    fclose(fp);
    pause();
}
//funcao para deposito
void efetuaDeposito(){
    FILE *fp;
    char nome_arq[TAM_NOME_ARQUIVO];
    int teste_senha, senhaLen;
    char teste_id[TAM_ID];
    char auxiliar[20];

    // Obtendo os dados do usuário
    do {
        printf("Insira o numero da sua conta (xxxxxx-x)\n>");
        scanf("%s", auxiliar);
        limpabuffer();
    } while (strlen(auxiliar) > TAM_ID);
    strcpy(teste_id, auxiliar);

    snprintf(nome_arq, TAM_NOME_ARQUIVO, "conta%s.txt", teste_id);

    fp = fopen(nome_arq, "rb");
    if(fp == NULL){
        printf("Ocorreu um erro ao abrir o arquivo ou essa conta nao existe!\n");
    }
    else{
        do{
            printf("Insira a senha numerica de 6 digitos da conta:\n>");
            scanf("%d", &teste_senha);
            limpabuffer();
            senhaLen = snprintf(NULL, 0, "%d", teste_senha);
            if(senhaLen != 6)  printf("Numero de digitos invalido, por favor, insira uma senha numerica de 6 digitos!\n");
        }while(senhaLen != 6);

        fread(&conta, sizeof(conta), 1, fp);

        if(teste_senha != conta.senha) printf("Senha incorreta, tente novamente!\n");
        else{
            double valor;
            do{
                printf("Insira o valor que deseja depositar:\n>");
                scanf("%lf", &valor);
                limpabuffer();
                if(valor <= 0)
                    printf("Nao e possivel depositar R$0.00 ou valores negativos! Insira um valor valido!\n");
            }while(valor <= 0);
            //alterando saldo na variavel
            conta.saldo += valor;
            fclose(fp);
            fp = fopen(nome_arq, "wb");
            if (fp == NULL) printf("Erro ao abrir o arquivo!\n");
            else{//alterando saldo no arquivo -> sobreescrevendo
                fwrite(&conta, sizeof(conta), 1, fp);
                fclose(fp);
                printf("Deposito realizado com sucesso!");
                printf("Saldo atual da conta: R$%.2lf\n", conta.saldo);
            }
            fclose(fp);
        }
    }
    fclose(fp);
    pause();
}
//funcao de saque
void efetuaSaque(){
    FILE *fp;
    char nome_arq[TAM_NOME_ARQUIVO];
    int teste_senha, senhaLen;
    char teste_id[TAM_ID];
    char auxiliar[20];

    // Obtendo os dados do usuário
    do {
        printf("Insira o numero da sua conta (xxxxxx-x)\n>");
        scanf("%s", auxiliar);
        limpabuffer();
    } while (strlen(auxiliar) > TAM_ID);
    strcpy(teste_id, auxiliar);

    snprintf(nome_arq, TAM_NOME_ARQUIVO, "conta%s.txt", teste_id);

    fp = fopen(nome_arq, "rb");
    if(fp == NULL){
        printf("Ocorreu um erro ao abrir o arquivo ou essa conta nao existe!\n");
    }
    else{
        do{
            printf("Insira a senha numerica de 6 digitos da conta:\n>");
            scanf("%d", &teste_senha);
            limpabuffer();
            senhaLen = snprintf(NULL, 0, "%d", teste_senha);
            if(senhaLen != 6)  printf("Numero de digitos invalido, por favor, insira uma senha numerica de 6 digitos!\n");
        }while(senhaLen != 6);

        fread(&conta, sizeof(conta), 1, fp);

        if(teste_senha != conta.senha) printf("Senha incorreta, tente novamente!\n");
        else{
            double valor;
            do{
                printf("Insira o valor que deseja sacar:\n>");
                scanf("%lf", &valor);
                limpabuffer();
                if(valor <= 0) printf("Nao e possivel sacar R$0.00 ou valores negativos! Insira um valor valido!\n");
                if(valor > conta.saldo) printf("Saldo da conta inferior ao valor que deseja sacar, por favor insira um valor valido!");
            }while(valor <= 0 || valor > conta.saldo);
            //alterando saldo na variavel
            conta.saldo -= valor;
            fclose(fp);
            fp = fopen(nome_arq, "wb");
            if (fp == NULL) printf("Erro ao abrir o arquivo!\n");
            else{//alterando saldo no arquivo -> sobreescrevendo
                fwrite(&conta, sizeof(conta), 1, fp);
                fclose(fp);
                printf("Saque realizado com sucesso!");
                printf("Saldo atual da conta: R$%.2lf\n", conta.saldo);
            }
            fclose(fp);
        }
    }
    fclose(fp);
    pause();
}
//funcao para atendimento
void atendimento(No **senhas){
    //verificando existencia de senhas na fila
    if (*senhas == NULL) {
        printf("Nenhuma senha para atender.\n");
        pause();
        return;
    }

    char codigoServico[3];
    No* p = *senhas;
    verificaCodigo(p->senha, codigoServico);//verificando o tipo de serviço e salvando
    printf("Atendendo senha: %s\n", p->senha);
    *senhas = excluir_senha(*senhas, p->senha, 2);
    //direcionando para as funcoes dos servicos especificos com base no codigo da senha
    if(strcmp(codigoServico, CODIGO_CONTA) == 0){
        gerenciarConta();
    }
    else if(strcmp(codigoServico, CODIGO_TRANSF) == 0){
        transferencia();
    }
    else if(strcmp(codigoServico, CODIGO_SALDO) == 0){
        consultaSaldo();
    }
    else if(strcmp(codigoServico, CODIGO_DEPOSITO) == 0){
        efetuaDeposito();
    }
    else if(strcmp(codigoServico, CODIGO_SAQUE) == 0){
        efetuaSaque();
    }
    else{//caso prioritario, pergunta o serviço e direciona para as funcoes dos serviços especificos
        int op_serv;
        do{
            op_serv = menu_servico();

            if (op_serv < 1 || op_serv > 5) {
                printf("Por favor, insira uma opcao valida\n");
            }
        }while (op_serv < 1 || op_serv > 5);

        switch (op_serv) {
            case 1:
                gerenciarConta();
            break;
            case 2:
                transferencia();
            break;
            case 3:
                consultaSaldo();
            break;
            case 4:
                efetuaDeposito();
            break;
            case 5:
                efetuaSaque();
            break;
        }
    }
}
//funcao de controle de contas
void controleContas(){
    char teste_user[100];
    char teste_senha[50];
    //obtendo dados do usuario para validar permissao do mesmo
    printf("AREA RESTRITA APENAS PARA USUARIOS AUTORIZADOS!\n\n");
    printf("Insira o usuario:\n>");
    scanf("%s", teste_user);
    limpabuffer();

    if(strcmp(teste_user, user_adm) != 0) printf("Usuario incorreto!\n");
    else{
        printf("Insira a senha:\n>");
        limpabuffer();
        scanf("%s", teste_senha);
        limpabuffer();
        if(strcmp(teste_senha, senha_adm) != 0) printf("Senha incorreta!\n");
        else{//caso permissao seja aceita, abre o arquivo de contas
            FILE *fp_contas;
            fp_contas = fopen("contas.txt", "r");
            if(fp_contas == NULL) printf("Erro ao abrir o arquivo!");
            else{//le todas as contas pelos ids no arquivo e mostra na tela
                char nome_arq[TAM_NOME_ARQUIVO];
                char ids[TAM_ID];
                FILE *fp;
                limpatela();
                printf("========CONTAS========\n\n");
                while(fscanf(fp_contas, "%s", ids) != EOF){
                    snprintf(nome_arq, TAM_NOME_ARQUIVO, "conta%s.txt", ids);
                    fp = fopen(nome_arq, "rb");
                    if(fp == NULL){//caso o id exista no aquivo de contas mas nao exista o arquivo particular, essa conta esta inativa pois ja foi excluida
                        printf("Conta %s inativa!\n\n", ids);
                        printf("----------------\n\n");
                    }
                    else{
                        fread(&conta, sizeof(conta), 1, fp);
                        printf("Titular: %s\n", conta.titular);
                        printf("Numero da conta: %s\n", conta.id);
                        printf("Senha: %d\n", conta.senha);
                        printf("Saldo: R$%.2lf\n\n", conta.saldo);
                        printf("----------------\n\n");
                    }
                    fclose(fp);
                }
            }
            fclose(fp_contas);
        }
    }
    pause();
}
//funcao principal
int main(){
    
    srand(time(NULL));//inicializando seed da funcao rand para o relogio atual
    No* senhas = NULL;
    int op;

    do{
        op = menu();

        switch (op){
            case 1:
                gerar_senha(&senhas, consulta_prioridade());     
            break;

            case 2:
                atendimento(&senhas);
            break;

            case 3:
                listar_senhas(senhas);
            break;

            case 4:
                char codigo[TAM_CHAMADA_SENHA];
                printf("Digite a senha a ser excluida: ");
                scanf("%s", codigo);
                limpabuffer();
                senhas = excluir_senha(senhas, codigo, 1);
            break;

            case 5:
                controleContas();
            break;

            case 6:
                printf("Saindo...\n");
            break;

            default:
                printf("Por favor, selecione uma opcao valida!\n");
                pause();
            break;
        }
    }while(op != 6);

    return 0;
}