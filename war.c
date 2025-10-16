#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// Diretivas de pré-processador para definir constantes
// #define realiza substituição textual antes da compilação
// Por convenção, constantes definidas com #define usam letras maiúsculas
#define TERRITORIOS_MAX 5
#define MISSOES_MAX 5
#define STRINGS_MAX_LENGTH 100

// Definição da struct Territorio
// Representa um território no mapa do jogo com nome, cor do exército e tropas
typedef struct {
    char nome[30];      // Nome do território
    char cor[10];       // Cor do exército que o controla
    int tropas;         // Número de tropas presentes
} Territorio;

// Estrutura para representar um jogador
typedef struct {
    char nome[30];      // Nome do jogador
    char* missao;       // Ponteiro para a missão alocada dinamicamente
    char cor[10];       // Cor do exército do jogador
} Jogador;

// ==================== PROTÓTIPOS DAS FUNÇÕES ====================

void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador);
void exibirMissao(char* missao, char* nomeJogador);
void atacar(Territorio* atacante, Territorio* defensor);
void exibirMapa(Territorio* mapa, int tamanho);
void liberarMemoria(Territorio* mapa, Jogador* jogadores, int numJogadores);
int validarAtaque(Territorio* atacante, Territorio* defensor, char* corJogador);

// ==================== FUNÇÃO PRINCIPAL ====================

int main() {
    setlocale(LC_ALL, "Portuguese");
    
    // Inicializa o gerador de números aleatórios com a hora atual
    // Isso garante sequências diferentes em cada execução do programa
    srand(time(NULL));
    
    // Declaração do vetor de missões possíveis usando MISSOES_MAX
    // Cada missão representa um objetivo estratégico diferente
    char* missoes[MISSOES_MAX] = {
        "Conquistar 3 territórios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Controlar pelo menos 5 territórios",
        "Acumular 50 tropas em territórios sob seu controle",
        "Conquistar 2 territórios com mais de 10 tropas cada"
    };
    
    // Alocação dinâmica do mapa usando TERRITORIOS_MAX
    // calloc inicializa toda memória com zeros, evitando lixo de memória
    Territorio* mapa = (Territorio*)calloc(TERRITORIOS_MAX, sizeof(Territorio));
    
    // Verifica se a alocação foi bem-sucedida
    if (mapa == NULL) {
        printf("Erro: Falha na alocação de memória para o mapa!\n");
        return 1;
    }
    
    // Inicialização dos territórios do mapa
    strcpy(mapa[0].nome, "Brasil"); strcpy(mapa[0].cor, "azul"); mapa[0].tropas = 10;
    strcpy(mapa[1].nome, "Argentina"); strcpy(mapa[1].cor, "verde"); mapa[1].tropas = 8;
    strcpy(mapa[2].nome, "Chile"); strcpy(mapa[2].cor, "vermelho"); mapa[2].tropas = 6;
    strcpy(mapa[3].nome, "Peru"); strcpy(mapa[3].cor, "amarelo"); mapa[3].tropas = 7;
    strcpy(mapa[4].nome, "Colômbia"); strcpy(mapa[4].cor, "azul"); mapa[4].tropas = 9;
    
    // Configuração dos jogadores
    int numJogadores = 2;
    Jogador* jogadores = (Jogador*)malloc(numJogadores * sizeof(Jogador));
    
    if (jogadores == NULL) {
        printf("Erro: Falha na alocação de memória para jogadores!\n");
        free(mapa);
        return 1;
    }
    
    // Inicializa dados dos jogadores
    strcpy(jogadores[0].nome, "Jogador 1");
    strcpy(jogadores[0].cor, "azul");
    strcpy(jogadores[1].nome, "Jogador 2");
    strcpy(jogadores[1].cor, "verde");
    
    // Aloca memória para as missões usando STRINGS_MAX_LENGTH
    // malloc aloca o espaço necessário sem inicializar (será preenchido depois)
    for (int i = 0; i < numJogadores; i++) {
        jogadores[i].missao = (char*)malloc(STRINGS_MAX_LENGTH * sizeof(char));
        if (jogadores[i].missao == NULL) {
            printf("Erro: Falha na alocação de memória para missão!\n");
            liberarMemoria(mapa, jogadores, i);
            return 1;
        }
        
        // Atribui uma missão aleatória usando MISSOES_MAX
        // Passa o ponteiro por referência para modificar o conteúdo
        atribuirMissao(jogadores[i].missao, missoes, MISSOES_MAX);
        
        // Exibe a missão apenas uma vez no início
        // Passa o ponteiro por valor apenas para leitura
        exibirMissao(jogadores[i].missao, jogadores[i].nome);
    }
    
    printf("\n=== ESTADO INICIAL DO MAPA ===\n");
    exibirMapa(mapa, TERRITORIOS_MAX);
    
    // Simulação de alguns turnos de jogo
    printf("\n=== SIMULAÇÃO DE TURNOS ===\n\n");
    
    for (int turno = 1; turno <= 3; turno++) {
        printf("--- TURNO %d ---\n", turno);
        
        // Cada jogador realiza um ataque
        for (int i = 0; i < numJogadores; i++) {
            printf("\n%s ataca...\n", jogadores[i].nome);
            
            // Encontra território do atacante (primeiro território da sua cor)
            int idxAtacante = -1, idxDefensor = -1;
            for (int j = 0; j < TERRITORIOS_MAX; j++) {
                if (strcmp(mapa[j].cor, jogadores[i].cor) == 0 && mapa[j].tropas > 1) {
                    idxAtacante = j;
                    break;
                }
            }
            
            // Encontra território inimigo para atacar
            for (int j = 0; j < TERRITORIOS_MAX; j++) {
                if (strcmp(mapa[j].cor, jogadores[i].cor) != 0) {
                    idxDefensor = j;
                    break;
                }
            }
            
            // Valida e executa o ataque se possível
            if (idxAtacante != -1 && idxDefensor != -1) {
                if (validarAtaque(&mapa[idxAtacante], &mapa[idxDefensor], jogadores[i].cor)) {
                    atacar(&mapa[idxAtacante], &mapa[idxDefensor]);
                }
            }
        }
        
        exibirMapa(mapa, TERRITORIOS_MAX);
        
        // Verificação silenciosa das missões ao final de cada turno
        for (int i = 0; i < numJogadores; i++) {
            if (verificarMissao(jogadores[i].missao, mapa, TERRITORIOS_MAX, jogadores[i].cor)) {
                printf("\n*** %s CUMPRIU SUA MISSÃO E VENCEU O JOGO! ***\n", jogadores[i].nome);
                printf("Missão cumprida: %s\n", jogadores[i].missao);
                liberarMemoria(mapa, jogadores, numJogadores);
                return 0;
            }
        }
    }
    
    printf("\n=== JOGO FINALIZADO SEM VENCEDOR ===\n");
    
    // Liberação de toda memória alocada dinamicamente
    liberarMemoria(mapa, jogadores, numJogadores);
    
    return 0;
}

// ==================== IMPLEMENTAÇÃO DAS FUNÇÕES ====================

/**
 * Função: atribuirMissao
 * Descrição: Sorteia uma missão aleatória e copia para a variável do jogador
 * Parâmetros:
 *   - destino: ponteiro para onde a missão será copiada (passagem por referência)
 *   - missoes: array com todas as missões disponíveis
 *   - totalMissoes: número total de missões no array
 */
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    // Gera um índice aleatório entre 0 e totalMissoes-1
    int indice = rand() % totalMissoes;
    
    // Copia a missão sorteada para o destino usando strcpy
    // strcpy copia caractere por caractere até encontrar o '\0'
    strcpy(destino, missoes[indice]);
}

/**
 * Função: verificarMissao
 * Descrição: Verifica se a missão do jogador foi cumprida
 * Parâmetros:
 *   - missao: string contendo a missão a ser verificada (passagem por valor)
 *   - mapa: ponteiro para o array de territórios
 *   - tamanho: número de territórios no mapa
 *   - corJogador: cor do exército do jogador
 * Retorno: 1 se a missão foi cumprida, 0 caso contrário
 */
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador) {
    // Verifica "Conquistar 3 territórios seguidos"
    if (strstr(missao, "3 territórios seguidos") != NULL) {
        int sequencia = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                sequencia++;
                if (sequencia >= 3) return 1;
            } else {
                sequencia = 0;
            }
        }
    }
    
    // Verifica "Eliminar todas as tropas da cor vermelha"
    if (strstr(missao, "cor vermelha") != NULL) {
        int temVermelho = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "vermelho") == 0) {
                temVermelho = 1;
                break;
            }
        }
        if (!temVermelho) return 1;
    }
    
    // Verifica "Controlar pelo menos 5 territórios"
    if (strstr(missao, "5 territórios") != NULL) {
        int contador = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                contador++;
            }
        }
        if (contador >= 5) return 1;
    }
    
    // Verifica "Acumular 50 tropas"
    if (strstr(missao, "50 tropas") != NULL) {
        int totalTropas = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                totalTropas += mapa[i].tropas;
            }
        }
        if (totalTropas >= 50) return 1;
    }
    
    // Verifica "Conquistar 2 territórios com mais de 10 tropas"
    if (strstr(missao, "2 territórios com mais de 10") != NULL) {
        int contador = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas > 10) {
                contador++;
            }
        }
        if (contador >= 2) return 1;
    }
    
    return 0;
}

/**
 * Função: exibirMissao
 * Descrição: Exibe a missão atribuída ao jogador
 * Parâmetros:
 *   - missao: string contendo a missão (passagem por valor para leitura)
 *   - nomeJogador: nome do jogador
 */
void exibirMissao(char* missao, char* nomeJogador) {
    printf("\n=== MISSÃO DE %s ===\n", nomeJogador);
    printf("Objetivo: %s\n", missao);
}

/**
 * Função: atacar
 * Descrição: Simula um ataque entre dois territórios usando rolagem de dados
 * Parâmetros:
 *   - atacante: ponteiro para o território atacante (passagem por referência)
 *   - defensor: ponteiro para o território defensor (passagem por referência)
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    // Simula rolagem de dados de 1 a 6 para atacante e defensor
    // rand() % 6 gera números de 0 a 5, somamos 1 para ter 1 a 6
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;
    
    printf("%s (dados: %d) ataca %s (dados: %d)...\n", 
           atacante->nome, dadoAtacante, defensor->nome, dadoDefensor);
    
    // Atacante vence se seu dado for maior
    if (dadoAtacante > dadoDefensor) {
        printf("-> Ataque bem-sucedido! %s conquistou %s\n", 
               atacante->nome, defensor->nome);
        
        // Transfere a cor do atacante para o defensor
        strcpy(defensor->cor, atacante->cor);
        
        // Transfere metade das tropas do atacante (arredonda para baixo)
        int tropasTransferidas = atacante->tropas / 2;
        atacante->tropas -= tropasTransferidas;
        defensor->tropas = tropasTransferidas;
    } else {
        printf("-> Ataque falhou! %s perde uma tropa\n", atacante->nome);
        
        // Atacante perde uma tropa em caso de falha
        if (atacante->tropas > 1) {
            atacante->tropas--;
        }
    }
}

/**
 * Função: exibirMapa
 * Descrição: Exibe o estado atual de todos os territórios no mapa
 * Parâmetros:
 *   - mapa: ponteiro para o array de territórios
 *   - tamanho: número de territórios no mapa
 */
void exibirMapa(Territorio* mapa, int tamanho) {
    printf("\n%-20s %-15s %-10s\n", "TERRITÓRIO", "COR", "TROPAS");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%-20s %-15s %-10d\n", mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("\n");
}

/**
 * Função: validarAtaque
 * Descrição: Valida se o ataque é permitido (apenas territórios inimigos)
 * Parâmetros:
 *   - atacante: ponteiro para o território atacante
 *   - defensor: ponteiro para o território defensor
 *   - corJogador: cor do jogador que está atacando
 * Retorno: 1 se o ataque é válido, 0 caso contrário
 */
int validarAtaque(Territorio* atacante, Territorio* defensor, char* corJogador) {
    // Verifica se o atacante pertence ao jogador
    if (strcmp(atacante->cor, corJogador) != 0) {
        printf("Erro: Você não controla %s!\n", atacante->nome);
        return 0;
    }
    
    // Verifica se o defensor é inimigo
    if (strcmp(defensor->cor, corJogador) == 0) {
        printf("Erro: Não pode atacar seu próprio território!\n");
        return 0;
    }
    
    // Verifica se o atacante tem tropas suficientes
    if (atacante->tropas <= 1) {
        printf("Erro: %s não tem tropas suficientes para atacar!\n", atacante->nome);
        return 0;
    }
    
    return 1;
}

/**
 * Função: liberarMemoria
 * Descrição: Libera toda a memória alocada dinamicamente no programa
 * Parâmetros:
 *   - mapa: ponteiro para o array de territórios
 *   - jogadores: ponteiro para o array de jogadores
 *   - numJogadores: número de jogadores para liberar missões
 * Observação: Essencial para evitar vazamentos de memória (memory leaks)
 */
void liberarMemoria(Territorio* mapa, Jogador* jogadores, int numJogadores) {
    // Libera as missões de cada jogador
    for (int i = 0; i < numJogadores; i++) {
        if (jogadores[i].missao != NULL) {
            free(jogadores[i].missao);
        }
    }
    
    // Libera o array de jogadores
    if (jogadores != NULL) {
        free(jogadores);
    }
    
    // Libera o mapa
    if (mapa != NULL) {
        free(mapa);
    }
    
    printf("\n[Sistema] Memória liberada com sucesso.\n");
}
