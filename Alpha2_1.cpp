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
    char titular[50];
    char id_conta[13];
    char senha[6]; 
}contas;

int cont_pr = 0;
int cont_gc = 0;
int cont_gt = 0;
int cont_cs = 0;
int cont_total = 0;
int cont_primos = 0;

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

char* encrypt(char senha[]){
    int senha_num = atoi(senha);
    int aux_codigo = 0, cont_potencia = 0, aux_atual, cont_div_exatas, i, j;

    while (senha_num!=0) { 
        aux_atual = senha_num%10;
        senha_num/=10;
        aux_codigo +=pow(aux_atual, cont_potencia);
        cont_potencia++;
    }

    aux_atual = cont_primos;

    for(i = 1; cont_primos == aux_atual; i++){
        cont_div_exatas = 0;
        for(j = 1; j<=i; j++){
            if((j % i == 0) && (j != i) && (i != 1)){
                
            }
            else{
                cont_div_exatas++;
            }
        }
        if(cont_div_exatas == 2){
            cont_primos++;
        }
    }

    senha_num = aux_codigo + i;

    sprintf(senha, "%06d", senha_num);

    return senha;
}

void abrirConta(char nome_arq[], struct Contas* contas) {
    printf("Debug: Iniciando a função abrirConta\n");

    printf("Insira o nome do titular: ");
    scanf("%[^\n]s", contas->titular);
    limpabuffer();
    printf("Debug: Nome do titular inserido: %s\n", contas->titular);

    do {
        printf("Insira uma senha de 6 digitos para a conta, %s: ", contas->titular);
        scanf("%[^\n]s", contas->senha);
        limpabuffer();
        printf("Debug: Senha inserida: %s\n", contas->senha);
    } while (strlen(contas->senha) != 6);

    strcpy(contas->senha, encrypt(contas->senha));
    printf("Debug: Senha criptografada: %s\n", contas->senha);

    long int aux = rand();
    sprintf(contas->id_conta, "%012ld", aux);
    printf("Debug: ID da conta gerado: %s\n", contas->id_conta);

    FILE *fp = fopen(nome_arq, "r");
    if (fp == NULL) {
        fp = fopen(nome_arq, "w+");
        if (fp == NULL) {
            printf("Erro ao criar o arquivo.\n");
            exit(1);
        }
        printf("Debug: Arquivo criado\n");
        fprintf(fp, "%s, %s, %s\n", contas->titular, contas->id_conta, contas->senha);
        fclose(fp);
        printf("Debug: Dados gravados no arquivo\n");
    } else {
        fclose(fp);
        fp = fopen(nome_arq, "a+");
        if (fp == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            exit(1);
        }
        printf("Debug: Arquivo aberto\n");
        fprintf(fp, "%s, %s, %s\n", contas->titular, contas->id_conta, contas->senha);
        fclose(fp);
        printf("Debug: Dados gravados no arquivo\n");
    }

    printf("Conta aberta com sucesso!\n");
    pause();
}



void encerrarConta(){

}

void gerenciarConta(struct Contas* contas) {
    int op;
    do {
        printf("1. Abrir conta\n2. Encerrar conta: ");
        scanf("%d", &op);
        limpabuffer();
        if (op != 1 && op != 2) {
            printf("Por favor, insira uma opcao valida!\n");
            pause();
        }
    } while (op != 1 && op != 2);

    if (op == 1) abrirConta("contas.txt", contas);
    if (op == 2) encerrarConta();
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

    if (strcmp(codigoServico, CODIGO_CONTA) == 0) {
        gerenciarConta(&contas);
    }
    else if (strcmp(codigoServico, CODIGO_SALDO) == 0) {
        // Adicionar funcionalidade para consultar saldo
    }
    else if (strcmp(codigoServico, CODIGO_TRANSF) == 0) {
        // Adicionar funcionalidade para transferências
    }
    else {
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
                gerenciarConta(&contas);
                break;
            case 2:
                // Adicionar funcionalidade para transferências
                break;
            case 3:
                // Adicionar funcionalidade para consultar saldo
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