#include <stdio.h>
#include <string.h>
#include <locale.h>

#define TERRITORIOS_MAX 5
#define MISSOES_MAX 3
#define STRINGS_MAX_LENGTH 100


// Definição da struct Territorio
// Uma struct em C permite agrupar diferentes tipos de dados relacionados
// em uma única estrutura, facilitando a organização e manipulação dos dados
typedef struct {
    char nome[STRINGS_MAX_LENGTH];      // Armazena o nome do território (até 29 caracteres + '\0')
    char cor[STRINGS_MAX_LENGTH];       // Armazena a cor do exército (até 9 caracteres + '\0')
    int tropas;         // Armazena o número de tropas no território
} Territorio;

// Função para limpar o buffer de entrada
// Necessária após scanf() para evitar problemas com entradas subsequentes
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    // Define o locale para português para suportar acentuação
    setlocale(LC_ALL, "Portuguese");
    
    // Declaração de um vetor (array) de structs
    // Este vetor armazenará os dados de 5 territórios
    Territorio territorios[TERRITORIOS_MAX];
    
    // Mensagem de boas-vindas e instruções ao usuário
    printf("=== SISTEMA DE CADASTRO DE TERRITÓRIOS ===\n\n");
    printf("Você irá cadastrar 5 territórios.\n");
    printf("Para cada território, informe: nome, cor do exército e número de tropas.\n\n");
    
    // Laço de entrada de dados
    // Percorre o vetor de structs e preenche cada elemento com os dados do usuário
    for (int i = 0; i < TERRITORIOS_MAX; i++) {
        printf("--- Território %d ---\n", i + 1);
        
        // Leitura do nome do território
        // Utiliza fgets para ler strings com espaços de forma segura
        printf("Nome do território: ");
        fgets(territorios[i].nome, STRINGS_MAX_LENGTH, stdin);
        // Remove o caractere de nova linha (\n) que o fgets captura
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';
        
        // Leitura da cor do exército
        printf("Cor do exército: ");
        fgets(territorios[i].cor, STRINGS_MAX_LENGTH, stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';
        
        // Leitura do número de tropas
        // Utiliza scanf para ler valores numéricos inteiros
        printf("Número de tropas: ");
        scanf("%d", &territorios[i].tropas);
        limparBuffer(); // Limpa o buffer após scanf
        
        printf("\n");
    }
    
    // Exibição dos dados cadastrados
    // Percorre o vetor de structs e exibe os dados de cada território
    printf("\n=== TERRITÓRIOS CADASTRADOS ===\n\n");
    
    for (int i = 0; i < 5; i++) {
        // Formatação clara dos dados para facilitar a leitura
        printf("Território %d:\n", i + 1);
        printf("  Nome: %s\n", territorios[i].nome);
        printf("  Cor do Exército: %s\n", territorios[i].cor);
        printf("  Tropas: %d\n", territorios[i].tropas);
        printf("\n");
    }
    
    printf("=== CADASTRO FINALIZADO COM SUCESSO ===\n");
    
    return 0;
}
