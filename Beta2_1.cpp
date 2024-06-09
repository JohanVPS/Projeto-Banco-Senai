#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// #define TAM_NOME 50
#define TAM_CHAMADA_SENHA 5
#define CODIGO_PRIORIDADE "PR"
#define CODIGO_CONTA "GC"
#define CODIGO_TRANSF "GT"
#define CODIGO_SALDO "CS"

#define TAM_TITULAR 100
#define TAM_ID 9
#define TAM_SENHA 6
#define QTD_IDS 999

#define RAND_MAX 99999999

void limpabuffer(){
    fflush(stdin);
}
void pause(){
    system("pause");
}
void limpatela(){
    system("clear || cls");
}

typedef struct No{
    char senha[TAM_CHAMADA_SENHA];
    struct No* prox;
} No;

struct Contas{      
    char titular[TAM_TITULAR];
    char id[TAM_ID];
    char senha[TAM_SENHA];
    float saldo;
}contas;

int cont_pr = 0;
int cont_gc = 0;
int cont_gt = 0;
int cont_cs = 0;
int cont_total = 0;
int cont_ids = 0;

unsigned long ids_gerados[QTD_IDS];

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

//criando função para inserir no fim da lista
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

void gerar_senha(No** senhas, int prioridade) {
    char codigo[TAM_CHAMADA_SENHA + 1];

    if (prioridade) {
        cont_pr++;
        snprintf(codigo, TAM_CHAMADA_SENHA + 1, "%s%03d", CODIGO_PRIORIDADE, cont_pr); //o +1 no segundo parametro refere-se ao '\0' que é considerado um caracter pela funçao snprintf() 

        No* novo = (No*)malloc(sizeof(No));
        if (novo == NULL) {
            printf("Erro de alocação de memoria.\n");
            exit(1);
        }
        strcpy(novo->senha, codigo);
        novo->prox = NULL;

        if (*senhas == NULL || strncmp((*senhas)->senha, CODIGO_PRIORIDADE, strlen(CODIGO_PRIORIDADE)) != 0) {
            // Lista vazia ou a primeira senha não é prioridade
            novo->prox = *senhas;
            *senhas = novo;
        } else {
            // Encontrar a última senha de prioridade
            No* p = *senhas;
            while (p->prox != NULL && strncmp(p->prox->senha, CODIGO_PRIORIDADE, strlen(CODIGO_PRIORIDADE)) == 0) {
                p = p->prox;
            }
            novo->prox = p->prox;
            p->prox = novo;
        }
    } else {
        int op_serv;
        do {
            printf("Qual tipo de servico deseja realizar?\n");
            printf("1. Abertura/Fechamento de conta\n");
            printf("2. Transferencias\n");
            printf("3. Consultar saldo\n> ");
            scanf("%d", &op_serv);
            limpabuffer();

            if (op_serv < 1 || op_serv > 3) {
                printf("Por favor, insira uma opcao valida\n");
            }
        } while (op_serv < 1 || op_serv > 3);

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
        }
        *senhas = insere_fim(*senhas, codigo); // Inserir no fim
    }

    printf("Sua senha: %s\n", codigo);
    pause();
}

void listar_senhas(No* ini) {
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
    pause();
}

No* excluir_senha(No* ini, char code[], int type) {
    if (ini == NULL) {
        printf("Nenhuma senha para excluir.\n");
        return NULL;
    }

    No* atual = ini;
    No* anterior = NULL;

    while (atual != NULL && strcmp(atual->senha, code) != 0) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Senha %s nao encontrada.\n", code);
    } else {
        if (anterior == NULL) {
            ini = atual->prox; // Senha está no primeiro nó
        } else {
            anterior->prox = atual->prox; // Pular o nó a ser removido
        }
        free(atual);
        if(type == 1)
            printf("Senha %s excluida com sucesso.\n", code);        
    }
    pause();
    return ini;
}

void verificaCodigo(char *senha, char *codigoServico) {
    strncpy(codigoServico, senha, 2);
    codigoServico[2] = '\0';
}

void abrirConta(char nome_arq[], Contas contas){
    char auxiliar[200];
    do{
        printf("Insira o nome do titular com no maximo 50 digitos:\n>");
        scanf("%[^\n]s", auxiliar);
        limpabuffer();
    }while(strlen(auxiliar) > TAM_TITULAR);
    strcpy(contas.titular, auxiliar);

    do{
        printf("Insira uma senha numerica de 6 digitos para a conta:\n>");
        scanf("%[^\n]s", auxiliar);
        limpabuffer();
    }while(strlen(auxiliar) > TAM_SENHA);
    strcpy(contas.senha, auxiliar);

    printf("Insira o valor do deposito inicial:\n>");
    scanf("%f", &contas.saldo);
    limpabuffer();

    int aux = rand() %1000000, digito = rand() % 10;

    snprintf(contas.id, TAM_ID, "%06d-%d", aux, digito);

    FILE *fp= fopen(nome_arq, "r");
    if (fp == NULL){                     // Se não conseguir ler o arquivo (se o arquivo não está no local inforomado)
        fp = fopen(nome_arq,"w+");
        fprintf(fp,"%s, ",contas.id);
        fprintf(fp,"%s, ",contas.titular);
        fprintf(fp,"%s, ",contas.senha);
        fprintf(fp,"%.2f\n",contas.saldo);
        fclose(fp);// sai do programa
    }
    else{
        fclose(fp);
        fp = fopen(nome_arq,"a+");
        fprintf(fp,"%s, ",contas.id);
        fprintf(fp,"%s, ",contas.titular);
        fprintf(fp,"%s, ",contas.senha);
        fprintf(fp,"%.2f\n",contas.saldo);
        fclose(fp);// sai do programa
    }
}

void encerrarConta(char nome_arq[], struct Contas contas){
    FILE *fp, *fp_temp;
    char teste_titular[TAM_TITULAR], teste_senha[TAM_SENHA];
    char teste_id[TAM_ID];
    float teste_saldo;
    bool conta_encontrada = false;
    char auxiliar[200], anterior[200];

    // Obtendo os dados do usuário
    do{
        printf("Insira o numero da conta que deseja encerrar, sem espacos\n>");
        scanf("%s", auxiliar);
        limpabuffer();
    }while(strlen(auxiliar) > TAM_ID);
    strcpy(contas.id, auxiliar);

    do{
        printf("Insira o nome do titular da conta que deseja encerrar\n>");
        scanf("%[^\n]s", auxiliar);
        limpabuffer();
    }while(strlen(auxiliar) > TAM_TITULAR);
    strcpy(contas.titular, auxiliar);

    do{
        printf("Insira a senha da conta\n>");
        scanf("%[^\n]s", auxiliar);
        limpabuffer();
    }while(strlen(auxiliar) > TAM_SENHA);
    strcpy(contas.senha, auxiliar);

    // Abrindo o arquivo original para leitura
    fp = fopen(nome_arq, "r");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Abrindo o arquivo temporário para escrita
    fp_temp = fopen("temp.txt", "w+");
    if (fp_temp == NULL) {
        printf("Erro ao criar o arquivo temporário.\n");
        fclose(fp);
        return;
    }

    // Lendo e processando o arquivo original
    while (fscanf(fp, "%[^,], %[^,], %[^,], %f\n", teste_id, teste_titular, teste_senha, &teste_saldo) != EOF) {
        //printf("Lido do arquivo: ID=%s, Titular=%s, Senha=%s, Saldo=%.2f\n", teste_id, teste_titular, teste_senha, teste_saldo); // Mensagem de depuração
        fscanf(fp, "%[^\n]s\n", auxiliar);
        //printf("Lido do arquivo e colocado na auxiliar: %s\n", auxiliar);
        // Comparando os dados lidos com os dados fornecidos pelo usuário
        if (strcmp(teste_id, contas.id) == 0 && strcmp(teste_senha, contas.senha) == 0) {
            conta_encontrada = true;
            if (teste_saldo == 0) {
                printf("Conta %s encerrada com sucesso.\n", contas.id);
                // Não escreve a conta no arquivo temporário (exclui a conta)
            } else {
                printf("E necessario estar com o saldo zerado para encerrar a conta!\n");
                // Escreve a conta no arquivo temporário (não exclui a conta)
                fprintf(fp_temp, "%s\n", auxiliar);
            }
        } else if(strcmp(anterior, auxiliar) != 0){
            // Escreve a conta no arquivo temporário
            fprintf(fp_temp, "%s\n", auxiliar);
        }
        strcpy(anterior, auxiliar);
    }

    if (!conta_encontrada) {
        printf("Alguma(s) das informacoes inseridas esta(o) incorreta(s), por favor verifique e tente novamente!\n");
        remove("temp.txt"); // Remove o arquivo temporário
    } else {
        remove(nome_arq); // Remove o arquivo original
        rename("temp.txt", nome_arq); // Renomeia o arquivo temporário para o nome original
    }

    fclose(fp);
    fclose(fp_temp);

    pause();
}

void gerenciarConta(){
    int op;
    do{
        printf("1. Abrir conta\n2. Encerrar conta\n>");
        scanf("%d", &op);
        limpabuffer();
        if(op != 1 && op != 2){
            printf("Por favor, insira uma opcao valida!\n");
            pause();
        }
    }while(op != 1 && op != 2);

    if(op == 1) abrirConta("contas.txt", contas);
    if(op == 2) encerrarConta("contas.txt", contas);
}

void atendimento(No **senhas) {
    if (*senhas == NULL) {
        printf("Nenhuma senha para atender.\n");
        pause();
        return;
    }

    char codigoServico[3];
    No* p = *senhas;
    verificaCodigo(p->senha, codigoServico);
    printf("Atendendo senha: %s\n", p->senha);
    *senhas = excluir_senha(*senhas, p->senha, 2);

    if(strcmp(codigoServico, CODIGO_CONTA) == 0){
        gerenciarConta();
    }
    else if(strcmp(codigoServico, CODIGO_SALDO) == 0){

    }
    else if(strcmp(codigoServico, CODIGO_SALDO) == 0){

    }
    else{
        int op_serv;
        do {
            printf("Qual tipo de servico deseja realizar?\n");
            printf("1. Abertura/Fechamento de conta\n");
            printf("2. Transferencias\n");
            printf("3. Consultar saldo\n> ");
            scanf("%d", &op_serv);
            limpabuffer();

            if (op_serv < 1 || op_serv > 3) {
                printf("Por favor, insira uma opcao valida\n");
            }
        } while (op_serv < 1 || op_serv > 3);

        switch (op_serv) {
            case 1:
                
            break;
            case 2:
                
            break;
            case 3:
                
            break;
        }
    }
}

int main(){
    No* senhas = NULL;
    int op;

    do{
        limpatela();
        printf("======BEM VINDO AO JUQUINHA'S BANK======\n\n");
        printf("\tSelecione a opcao desejada: \n\n");
        printf("1.Gerar senha\n");
        printf("2.Efetuar atendimento\n");
        printf("3.Controle de senha\n");
        printf("4.Exclusao de senha\n");
        printf("5.Sair\n");
        printf("\n> ");
        scanf("%d", &op);
        limpabuffer();
        limpatela();

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
                printf("Digite a senha a ser excluída: ");
                scanf("%s", codigo);
                limpabuffer();
                senhas = excluir_senha(senhas, codigo, 1);
            break;

            case 5:
                printf("Saindo...\n");
            break;

            default:
                printf("Por favor, selecione uma opcao valida!\n");
                pause();
            break;
        }
    }while(op != 5);

    return 0;
}