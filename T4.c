#include "tela.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define BAR_WIDTH 300
#define BAR_HEIGHT 50
#define BAR_MARGIN 20
#define TAM 3
#define RNK 3

typedef struct 
{
    double pontuacao;
    char nome[TAM];
}Recorde;

typedef struct 
{
    int cor_alvo_r, cor_alvo_g, cor_alvo_b;  
    int cor_chute_r, cor_chute_g, cor_chute_b;  
    int cor_r, cor_g, cor_b;  
    int barra_corrente;  
    double start_time;  
    double elapsed_time;  
    Recorde recorde;
   
} GameState;

void desenhar_barras_rgb(GameState *game_state);
void desenhar_quadrados(GameState *game_state);
void atualizar_cores(GameState *game_state);
void gerar_cor_aleatoria(GameState *game_state);
void jogo_acertar_cor(GameState *game_state);
void mover_barra_cima(GameState *game_state);
void mover_barra_baixo(GameState *game_state);
void mover_barra_esquerda(GameState *game_state);
void mover_barra_direita(GameState *game_state);
void exibir_tempo_restante(GameState *game_state);
void get_nome(GameState *game_state);
void exibir_pontuacao(GameState *game_state);
void calcular_pontuacao(GameState *game_state);


void desenhar_barras_rgb(GameState *game_state) 
{
    int pos_x = 20;
    int pos_y = SCREEN_HEIGHT - (BAR_HEIGHT + BAR_MARGIN) * 3;

    // Desenha a barra vermelha
    tela_retangulo(pos_x, pos_y, pos_x + BAR_WIDTH, pos_y + BAR_HEIGHT, 2, branco, tela_cria_cor(game_state->cor_r / 255.0, 0, 0));
    int indicador_r_x = pos_x + game_state->cor_r * BAR_WIDTH / 255;
    tela_linha(indicador_r_x, pos_y, indicador_r_x, pos_y + BAR_HEIGHT, 2, branco);

    // Desenha a barra verde
    pos_y += BAR_HEIGHT + BAR_MARGIN;
    tela_retangulo(pos_x, pos_y, pos_x + BAR_WIDTH, pos_y + BAR_HEIGHT, 2, branco, tela_cria_cor(0, game_state->cor_g / 255.0, 0));
    int indicador_g_x = pos_x + game_state->cor_g * BAR_WIDTH / 255;
    tela_linha(indicador_g_x, pos_y, indicador_g_x, pos_y + BAR_HEIGHT, 2, branco);

    // Desenha a barra azul
    pos_y += BAR_HEIGHT + BAR_MARGIN;
    tela_retangulo(pos_x, pos_y, pos_x + BAR_WIDTH, pos_y + BAR_HEIGHT, 2, branco, tela_cria_cor(0, 0, game_state->cor_b / 255.0));
    int indicador_b_x = pos_x + game_state->cor_b * BAR_WIDTH / 255;
    tela_linha(indicador_b_x, pos_y, indicador_b_x, pos_y + BAR_HEIGHT, 2, branco);
}

void desenhar_quadrados(GameState *game_state) 
{
    int tela_centro_x = SCREEN_WIDTH / 2;
    int tela_centro_y = SCREEN_HEIGHT / 2;
    int quadrado_lado = 100;
    int distancia_entre_quadrados = 40;

    // Calcula as coordenadas do quadrado da cor aleatória
    int cor_aleatoria_x = tela_centro_x - quadrado_lado - distancia_entre_quadrados / 2;
    int cor_aleatoria_y = tela_centro_y - quadrado_lado / 2;

    // Desenha o quadrado da cor aleatória
    tela_retangulo(cor_aleatoria_x, cor_aleatoria_y, cor_aleatoria_x + quadrado_lado, cor_aleatoria_y + quadrado_lado, 2, branco, tela_cria_cor(game_state->cor_alvo_r / 255.0, game_state->cor_alvo_g / 255.0, game_state->cor_alvo_b / 255.0));

    // Calcula as coordenadas do quadrado do palpite
    int palpite_x = tela_centro_x + distancia_entre_quadrados / 2;
    int palpite_y = tela_centro_y - quadrado_lado / 2;

    // Desenha o quadrado do palpite
    tela_retangulo(palpite_x, palpite_y, palpite_x + quadrado_lado, palpite_y + quadrado_lado, 2, branco, tela_cria_cor(game_state->cor_r / 255.0, game_state->cor_g / 255.0, game_state->cor_b / 255.0));
}

void atualizar_cores(GameState *game_state) 
{
    desenhar_barras_rgb(game_state);
    desenhar_quadrados(game_state);
}

void gerar_cor_aleatoria(GameState *game_state) 
{
    game_state->cor_alvo_r = rand() % 256;  // Gera um número aleatório entre 0 e 255 para as cores
    game_state->cor_alvo_g = rand() % 256;  
    game_state->cor_alvo_b = rand() % 256;  
}



void get_nome(GameState *game_state) 
{
    int i = 0;
    char nome[TAM + 1]; // Adiciona 1 para o caractere nulo de terminação
    tela_texto_esq(400, 400, 20, branco, "Insira suas iniciais:");
    tela_texto_dir(410, 400, 20, branco, nome);
    tela_atualiza();

    while (true) {
        int tecla = tela_tecla();

        if (tecla == c_enter)
            break;
        if (tecla != c_none) {
            if (tecla == c_back) {
                if (i > 0) {
                    i--;
                    nome[i] = '\0';
                }
            } else {
                if (i < TAM) { // Ajusta a verificação para evitar estouro de buffer
                    nome[i] = tecla;
                    nome[i + 1] = '\0';
                    i++;
                }
            }
            tela_texto_esq(400, 400, 20, branco, "Insira suas iniciais:");
            tela_texto_dir(410, 400, 20, branco, nome);
            tela_atualiza();
        }
    }
    strncpy(game_state->recorde.nome, nome, sizeof(game_state->recorde.nome));
} 

double calcula_pontuacao(GameState *game_state) 
{
    // Calcula a diferença absoluta entre as cores
    int diff_r = abs(game_state->cor_alvo_r - game_state->cor_r);
    int diff_g = abs(game_state->cor_alvo_g - game_state->cor_g);
    int diff_b = abs(game_state->cor_alvo_b - game_state->cor_b);

    // Calcula a distância do chute
    int distancia_chute = diff_r + diff_g + diff_b;

    // Calcula a pontuação baseada na distância do chute
    double pontuacao = 100.0 - (distancia_chute * 100.0 / (255 * 3));

    // Calcula o bônus de antecipação
    double anticipation_time = 20.0 - game_state->elapsed_time;
    double bonus = pontuacao * (anticipation_time * 0.05);
    pontuacao += bonus;

    return pontuacao;
}
void salva_recorde(GameState *game_state) 
{
    FILE *arquivo = fopen("recorde.txt", "a");
    tela_atualiza();
    get_nome(game_state);
    if (arquivo != NULL) {
        game_state->recorde.pontuacao = calcula_pontuacao(game_state);
        fprintf(arquivo, "%s %.2lf\n", game_state->recorde.nome, game_state->recorde.pontuacao);
    }
    else {
        printf("Erro ao abrir o arquivo\n");
    }
    
}
void mover_barra_cima(GameState *game_state) 
{
    game_state->barra_corrente++;
    if (game_state->barra_corrente > 2) {
        game_state->barra_corrente = 2;
    }
}

void mover_barra_baixo(GameState *game_state) 
{
    game_state->barra_corrente--;
    if (game_state->barra_corrente < 0) {
        game_state->barra_corrente = 0;
    }
}

void mover_barra_esquerda(GameState *game_state) 
{
    if (game_state->barra_corrente == 2) {
        game_state->cor_r -= 10;
        if (game_state->cor_r < 0) {
            game_state->cor_r = 255;
        }
    } else if (game_state->barra_corrente == 1) {
        game_state->cor_g -= 10;
        if (game_state->cor_g < 0) {
            game_state->cor_g = 255;
        }
    } else if (game_state->barra_corrente == 0) {
        game_state->cor_b -= 10;
        if (game_state->cor_b < 0) 
        {
            game_state->cor_b = 255;
        }
    }
}

void mover_barra_direita(GameState *game_state) {
    if (game_state->barra_corrente == 2) {
        game_state->cor_r += 10;
        if (game_state->cor_r > 255) {
            game_state->cor_r = 0;
        }
    } else if (game_state->barra_corrente == 1) {
        game_state->cor_g += 10;
        if (game_state->cor_g > 255) {
            game_state->cor_g = 0;
        }
    } else if (game_state->barra_corrente == 0) {
        game_state->cor_b += 10;
        if (game_state->cor_b > 255) {
            game_state->cor_b = 0;
        }
    }
}

void exibir_resultado(GameState *game_state) {
    
   
    salva_recorde(game_state);
    printf("Cor sorteada: %d, %d, %d\n", game_state->cor_alvo_r, game_state->cor_alvo_g, game_state->cor_alvo_b);
    printf("Sua pontuação: %.1f pontos\n", game_state->recorde.pontuacao);
}

void jogo_acertar_cor(GameState *game_state) {
    bool sair = false;
    game_state->start_time = time(NULL);
    gerar_cor_aleatoria(game_state);

    while (!sair) {
        tela_atualiza();
        atualizar_cores(game_state);

        int tecla = tela_tecla();
        if (tecla == c_up) {
            mover_barra_cima(game_state);
        } else if (tecla == c_down) {
            mover_barra_baixo(game_state);
        } else if (tecla == c_left) {
            mover_barra_esquerda(game_state);
        } else if (tecla == c_right) {
            mover_barra_direita(game_state);
        } else if (tecla == c_enter || game_state->elapsed_time >= 20.0) {
            sair = true;  // Sai do loop se a tecla "Enter" for pressionada ou se o tempo passar de 20 segundos
            exibir_resultado(game_state);
        }

        // Verifica se 20 segundos se passaram
        game_state->elapsed_time = difftime(time(NULL), game_state->start_time);
        if (game_state->elapsed_time >= 20.0) {
            exibir_resultado(game_state);
        }
        
        exibir_tempo_restante(game_state);        
    }
}

void exibir_tempo_restante(GameState *game_state)
{
    int tela_centro_x = SCREEN_WIDTH / 2;
    int tela_centro_y = SCREEN_HEIGHT / 2;

    double tempo_restante = 20.0 - game_state->elapsed_time;
    char texto_tempo[40];
    snprintf(texto_tempo, sizeof(texto_tempo), "Tempo restante: %.0f segundos", tempo_restante);

    int tamanho_texto = 20;
    int pos_x = tela_centro_x - tamanho_texto / 2;
    int pos_y = tela_centro_y - tela_centro_y / 2;

    tela_texto(pos_x, pos_y,  tamanho_texto, branco, texto_tempo);
}

int main() {
    srand(time(NULL));
    GameState game_state;
    tela_inicio(SCREEN_WIDTH, SCREEN_HEIGHT, "Acerte a Cor");    
    jogo_acertar_cor(&game_state);
    tela_fim();
    return 0;
}
