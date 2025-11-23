#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Definição da estrutura Pessoa
typedef struct {
    char nome[100];
    char cpf[12]; // 11 dígitos + '\0'
    int idade;
    char email[100];
} Pessoa;

// Nome do arquivo binário de dados
#define NOME_ARQUIVO "cadastro_pessoas.dat"

// Protótipos das funções
void menu();
void cadastrarPessoa();
void listarPessoas();
void buscarPessoaPorCpf();
void atualizarPessoaPorCpf();
void removerPessoaPorCpf();
int verificarCpfDuplicado(const char *cpf);
int validarCpf(const char *cpf);

// Função principal
int main() {
    int opcao;

    do {
        menu();
        printf("Escolha uma opção: ");
        // Garante que a entrada é um número inteiro
        if (scanf("%d", &opcao) != 1) {
            // Limpa o buffer de entrada em caso de falha
            while (getchar() != '\n');
            opcao = 0; // Opção inválida
        } else {
            // Limpa o buffer de entrada após a leitura bem-sucedida
            while (getchar() != '\n');
        }

        switch (opcao) {
            case 1:
                cadastrarPessoa();
                break;
            case 2:
                listarPessoas();
                break;
            case 3:
                buscarPessoaPorCpf();
                break;
            case 4:
                atualizarPessoaPorCpf();
                break;
            case 5:
                removerPessoaPorCpf();
                break;
            case 6:
                printf("\nSaindo do programa. Até mais!\n");
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
                break;
        }
        printf("\nPressione ENTER para continuar...");
        getchar(); // Espera o usuário pressionar ENTER
        system("clear"); // Limpa a tela (pode ser substituído por uma função mais portátil se necessário)
    } while (opcao != 6);

    return 0;
}

// Exibe o menu de opções
void menu() {
    printf("======================================\n");
    printf("      SISTEMA DE CADASTRO DE PESSOAS\n");
    printf("======================================\n");
    printf("1. Cadastrar nova pessoa (Create)\n");
    printf("2. Listar todas as pessoas (Read)\n");
    printf("3. Buscar pessoa por CPF (Read)\n");
    printf("4. Atualizar dados por CPF (Update)\n");
    printf("5. Remover pessoa por CPF (Delete)\n");
    printf("6. Sair\n");
    printf("======================================\n");
}

// Valida se o CPF tem 11 dígitos e é composto apenas por números
int validarCpf(const char *cpf) {
    if (strlen(cpf) != 11) {
        return 0; // Tamanho incorreto
    }
    for (int i = 0; i < 11; i++) {
        if (!isdigit(cpf[i])) {
            return 0; // Contém caracteres não numéricos
        }
    }
    return 1; // CPF válido
}

// Verifica se um CPF já está cadastrado no arquivo
// Retorna 1 se duplicado, 0 caso contrário
int verificarCpfDuplicado(const char *cpf) {
    FILE *arquivo = fopen(NOME_ARQUIVO, "rb");
    if (arquivo == NULL) {
        return 0; // Arquivo não existe ou não pode ser aberto, então não há duplicidade
    }

    Pessoa p;
    int duplicado = 0;

    while (fread(&p, sizeof(Pessoa), 1, arquivo) == 1) {
        if (strcmp(p.cpf, cpf) == 0) {
            duplicado = 1;
            break;
        }
    }

    fclose(arquivo);
    return duplicado;
}

// 1. Cadastrar nova pessoa (Create)
void cadastrarPessoa() {
    Pessoa novaPessoa;
    char cpf_temp[100];

    printf("\n--- CADASTRO DE NOVA PESSOA ---\n");

    printf("Nome: ");
    fgets(novaPessoa.nome, sizeof(novaPessoa.nome), stdin);
    novaPessoa.nome[strcspn(novaPessoa.nome, "\n")] = 0; // Remove o '\n'

    do {
        printf("CPF (11 dígitos, apenas números): ");
        fgets(cpf_temp, sizeof(cpf_temp), stdin);
        cpf_temp[strcspn(cpf_temp, "\n")] = 0; // Remove o '\n'

        if (!validarCpf(cpf_temp)) {
            printf("Erro: CPF inválido. Deve ter 11 dígitos e conter apenas números.\n");
            continue;
        }

        if (verificarCpfDuplicado(cpf_temp)) {
            printf("Erro: CPF já cadastrado. O CPF deve ser único.\n");
        } else {
            strcpy(novaPessoa.cpf, cpf_temp);
            break;
        }
    } while (1);

    printf("Idade: ");
    // Garante que a entrada é um número inteiro
    while (scanf("%d", &novaPessoa.idade) != 1 || novaPessoa.idade <= 0) {
        printf("Erro: Idade inválida. Digite um número inteiro positivo: ");
        while (getchar() != '\n'); // Limpa o buffer
    }
    while (getchar() != '\n'); // Limpa o buffer após a leitura da idade

    printf("E-mail: ");
    fgets(novaPessoa.email, sizeof(novaPessoa.email), stdin);
    novaPessoa.email[strcspn(novaPessoa.email, "\n")] = 0; // Remove o '\n'

    FILE *arquivo = fopen(NOME_ARQUIVO, "ab"); // Abre para adicionar (append binary)
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }

    if (fwrite(&novaPessoa, sizeof(Pessoa), 1, arquivo) == 1) {
        printf("\nPessoa cadastrada com sucesso!\n");
    } else {
        printf("\nErro ao escrever os dados no arquivo.\n");
    }

    fclose(arquivo);
}

// 2. Listar todas as pessoas (Read)
void listarPessoas() {
    FILE *arquivo = fopen(NOME_ARQUIVO, "rb"); // Abre para leitura binária
    if (arquivo == NULL) {
        printf("\n--- LISTA DE PESSOAS ---\n");
        printf("Nenhum cadastro encontrado.\n");
        return;
    }

    Pessoa p;
    int contador = 0;

    printf("\n--- LISTA DE PESSOAS CADASTRADAS ---\n");
    printf("--------------------------------------------------------------------------------\n");
    printf("| %-3s | %-30s | %-11s | %-5s | %-30s |\n", "ID", "NOME", "CPF", "IDADE", "E-MAIL");
    printf("--------------------------------------------------------------------------------\n");

    while (fread(&p, sizeof(Pessoa), 1, arquivo) == 1) {
        printf("| %-3d | %-30s | %-11s | %-5d | %-30s |\n",
               ++contador, p.nome, p.cpf, p.idade, p.email);
    }

    printf("--------------------------------------------------------------------------------\n");

    if (contador == 0) {
        printf("Nenhum cadastro encontrado.\n");
    }

    fclose(arquivo);
}

// 3. Buscar pessoa por CPF (Read)
void buscarPessoaPorCpf() {
    FILE *arquivo = fopen(NOME_ARQUIVO, "rb");
    if (arquivo == NULL) {
        printf("\nNenhum cadastro encontrado.\n");
        return;
    }

    char cpf_busca[12];
    printf("\n--- BUSCAR PESSOA POR CPF ---\n");
    printf("Digite o CPF a ser buscado: ");
    fgets(cpf_busca, sizeof(cpf_busca), stdin);
    cpf_busca[strcspn(cpf_busca, "\n")] = 0;

    if (!validarCpf(cpf_busca)) {
        printf("Erro: CPF inválido. Deve ter 11 dígitos e conter apenas números.\n");
        fclose(arquivo);
        return;
    }

    Pessoa p;
    int encontrado = 0;

    while (fread(&p, sizeof(Pessoa), 1, arquivo) == 1) {
        if (strcmp(p.cpf, cpf_busca) == 0) {
            printf("\n--- PESSOA ENCONTRADA ---\n");
            printf("Nome:   %s\n", p.nome);
            printf("CPF:    %s\n", p.cpf);
            printf("Idade:  %d\n", p.idade);
            printf("E-mail: %s\n", p.email);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("\nErro: Pessoa com CPF %s não encontrada.\n", cpf_busca);
    }

    fclose(arquivo);
}

// 4. Atualizar dados por CPF (Update)
void atualizarPessoaPorCpf() {
    FILE *arquivo = fopen(NOME_ARQUIVO, "r+b"); // Abre para leitura e escrita binária
    if (arquivo == NULL) {
        printf("\nNenhum cadastro encontrado para atualizar.\n");
        return;
    }

    char cpf_busca[12];
    printf("\n--- ATUALIZAR CADASTRO POR CPF ---\n");
    printf("Digite o CPF da pessoa a ser atualizada: ");
    fgets(cpf_busca, sizeof(cpf_busca), stdin);
    cpf_busca[strcspn(cpf_busca, "\n")] = 0;

    if (!validarCpf(cpf_busca)) {
        printf("Erro: CPF inválido. Deve ter 11 dígitos e conter apenas números.\n");
        fclose(arquivo);
        return;
    }

    Pessoa p;
    long posicao = -1;
    int encontrado = 0;

    // Percorre o arquivo para encontrar o registro
    while (fread(&p, sizeof(Pessoa), 1, arquivo) == 1) {
        if (strcmp(p.cpf, cpf_busca) == 0) {
            encontrado = 1;
            posicao = ftell(arquivo) - sizeof(Pessoa); // Posição de início do registro
            break;
        }
    }

    if (!encontrado) {
        printf("\nErro: Pessoa com CPF %s não encontrada.\n", cpf_busca);
        fclose(arquivo);
        return;
    }

    printf("\nRegistro atual:\n");
    printf("Nome:   %s\n", p.nome);
    printf("Idade:  %d\n", p.idade);
    printf("E-mail: %s\n", p.email);

    printf("\nDigite os novos dados (deixe em branco para manter o valor atual):\n");

    // Novo Nome
    char novo_nome[100];
    printf("Novo Nome (%s): ", p.nome);
    fgets(novo_nome, sizeof(novo_nome), stdin);
    novo_nome[strcspn(novo_nome, "\n")] = 0;
    if (strlen(novo_nome) > 0) {
        strcpy(p.nome, novo_nome);
    }

    // Nova Idade
    char nova_idade_str[10];
    int nova_idade = -1;
    printf("Nova Idade (%d): ", p.idade);
    fgets(nova_idade_str, sizeof(nova_idade_str), stdin);
    nova_idade_str[strcspn(nova_idade_str, "\n")] = 0;
    if (strlen(nova_idade_str) > 0) {
        if (sscanf(nova_idade_str, "%d", &nova_idade) == 1 && nova_idade > 0) {
            p.idade = nova_idade;
        } else {
            printf("Aviso: Idade inválida. Mantendo a idade atual.\n");
        }
    }

    // Novo E-mail
    char novo_email[100];
    printf("Novo E-mail (%s): ", p.email);
    fgets(novo_email, sizeof(novo_email), stdin);
    novo_email[strcspn(novo_email, "\n")] = 0;
    if (strlen(novo_email) > 0) {
        strcpy(p.email, novo_email);
    }

    // Volta para a posição do registro e o sobrescreve
    fseek(arquivo, posicao, SEEK_SET);
    if (fwrite(&p, sizeof(Pessoa), 1, arquivo) == 1) {
        printf("\nCadastro atualizado com sucesso!\n");
    } else {
        printf("\nErro ao atualizar o cadastro.\n");
    }

    fclose(arquivo);
}

// 5. Remover pessoa por CPF (Delete)
void removerPessoaPorCpf() {
    char cpf_busca[12];
    printf("\n--- REMOVER PESSOA POR CPF ---\n");
    printf("Digite o CPF da pessoa a ser removida: ");
    fgets(cpf_busca, sizeof(cpf_busca), stdin);
    cpf_busca[strcspn(cpf_busca, "\n")] = 0;

    if (!validarCpf(cpf_busca)) {
        printf("Erro: CPF inválido. Deve ter 11 dígitos e conter apenas números.\n");
        return;
    }

    FILE *arquivo_origem = fopen(NOME_ARQUIVO, "rb");
    if (arquivo_origem == NULL) {
        printf("\nNenhum cadastro encontrado para remover.\n");
        return;
    }

    // Cria um arquivo temporário para armazenar os registros que não serão removidos
    FILE *arquivo_temp = fopen("temp.dat", "wb");
    if (arquivo_temp == NULL) {
        perror("Erro ao criar arquivo temporário");
        fclose(arquivo_origem);
        return;
    }

    Pessoa p;
    int encontrado = 0;

    // Copia todos os registros, exceto o que será removido, para o arquivo temporário
    while (fread(&p, sizeof(Pessoa), 1, arquivo_origem) == 1) {
        if (strcmp(p.cpf, cpf_busca) == 0) {
            encontrado = 1;
            printf("\nRegistro de %s (CPF: %s) encontrado e marcado para remoção.\n", p.nome, p.cpf);
        } else {
            fwrite(&p, sizeof(Pessoa), 1, arquivo_temp);
        }
    }

    fclose(arquivo_origem);
    fclose(arquivo_temp);

    if (!encontrado) {
        printf("\nErro: Pessoa com CPF %s não encontrada.\n", cpf_busca);
        remove("temp.dat"); // Remove o arquivo temporário
        return;
    }

    // Substitui o arquivo original pelo temporário
    if (remove(NOME_ARQUIVO) != 0) {
        perror("Erro ao remover o arquivo original");
        return;
    }

    if (rename("temp.dat", NOME_ARQUIVO) != 0) {
        perror("Erro ao renomear o arquivo temporário");
        return;
    }

    printf("\nPessoa removida com sucesso!\n");
}
