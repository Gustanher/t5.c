#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tela.h"
#include <string.h>

#define COL 5
#define LIN 5
#define LADO 100
#define TAM 20
#define TELA_X 800
#define TELA_Y 800

typedef struct
{
    char matriz[LIN][COL];
    bool fim;
    int pontos;
    char nome[TAM];
} controle;

int gera_posicao()
{
    int pos;
    pos = (rand() % COL) * COL + (rand() % LIN);
    return pos;
}

char gera_caracter()
{
    int letra;
    letra = rand() % 4;
    if (letra == 3)
    {
        return 'B';
    }
    else
    {
        return 'A';
    }
}

bool posicao_disponivel(controle *game)
{
    for (int i = 0; i < LIN; i++) {
        for (int j = 0; j < COL; j++) {
            if (game->matriz[i][j] == '0') {
                return true;
            }
        }
    }
    return false;    
}

void popula_matriz(controle *game)
{
    int r_line, r_col;

    if (posicao_disponivel(game)) {        
        do {
            r_line = gera_posicao();
            r_col = gera_posicao();
        } while (game->matriz[r_line][r_col] != '0');
        game->matriz[r_line][r_col] = gera_caracter();
    } 
}

void inicializa_matriz(controle *game)
{
    for (int i = 0; i < LIN; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            game->matriz[i][j] = '0';
        }
    }
    //gera duas posições inicialmente
    popula_matriz(game);
    popula_matriz(game);
}

int cor_do_bloco(char valor)
{
    if (valor == 'A')
    {
        return vermelho;
    }
    else if (valor == 'B')
    {
        return azul;
    }
    else if (valor == 'C')
    {
        return verde;
    }
    else if (valor == 'D')
    {
        return amarelo;
    }
    else if (valor == 'E')
    {
        return rosa;
    }
    else if (valor == 'F')
    {
        return laranja;
    }
    else
    {
        return transparente;
    }
}

void mostra_pontuação(controle *game)
{
    char pontos[20];
    sprintf(pontos, "Pontuação: %d", game->pontos);
    tela_texto(200, 50, 40, branco, pontos);
}

void imprime_na_tela(controle *game)
{
    int x = 150, y = 150;
    mostra_pontuação(game);
    for (int i = 0; i < LIN; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            char valor[2];
            int cor = cor_do_bloco(game->matriz[i][j]);
            sprintf(valor, "%c", game->matriz[i][j]);
            tela_retangulo(x - LADO / 2, y - LADO / 2, x + LADO / 2, y + LADO / 2, 2, branco, cor);
            tela_texto(x, y, 40, branco, valor);
            x += LADO;
        }
        x = 150;
        y += LADO;
    }
}

int calcula_pontuacao(char valor_matriz)
{
    int pontos = 0;
    if(valor_matriz == 'B')
    {
        pontos = 30;
    }else if(valor_matriz == 'C')
    {
        pontos = 90;
    }else if(valor_matriz == 'D')
    {
        pontos = 270;
    }else if(valor_matriz == 'E')
    {
        pontos = 810;
    }else if(valor_matriz == 'F')
    {
        pontos = 2430;
    }
    return pontos;
}
void move_right(controle *game)
{
    bool move_realizado = false;

    for (int i = 0; i < LIN; i++)
    {
        for (int j = COL - 2; j >= 0; j--)
        {
            if (game->matriz[i][j] != '0')
            {
                int k = j;
                while (k < COL - 1 && (game->matriz[i][k + 1] == '0' || game->matriz[i][k + 1] == game->matriz[i][k]))
                {
                    if (k + 2 < COL && game->matriz[i][k + 2] == game->matriz[i][k + 1] && game->matriz[i][k + 1] == game->matriz[i][k])
                    {
                        game->matriz[i][k + 2] = game->matriz[i][k] + 1;
                        game->matriz[i][k + 1] = '0';
                        game->matriz[i][k] = '0';
                        game->pontos += calcula_pontuacao(game->matriz[i][k + 2]);
                        move_realizado = true;
                        break;
                    }
                    else if(game->matriz[i][k + 1] == '0')
                   {
                        game->matriz[i][k + 1] = game->matriz[i][k];
                        game->matriz[i][k] = '0';
                        move_realizado = true;
                        game->pontos++;
                    }
                    k++;
                }
            }
        }
    }
    
    if (move_realizado)
    {
        popula_matriz(game);
    }
}
void move_up(controle *game)
{
    bool move_realizado = false;

    for (int j = 0; j < COL; j++)
    {
        for (int i = 1; i < LIN; i++)
        {
            if (game->matriz[i][j] != '0')
            {
                int k = i;
                while (k > 0 && (game->matriz[k - 1][j] == '0' || game->matriz[k - 1][j] == game->matriz[k][j]))
                {
                    if (k - 2 >= 0 && game->matriz[k - 2][j] == game->matriz[k - 1][j] && game->matriz[k - 1][j] == game->matriz[k][j])
                    {
                        game->matriz[k - 2][j] = game->matriz[k][j] + 1;
                        game->matriz[k - 1][j] = '0';
                        game->matriz[k][j] = '0';
                        game->pontos += calcula_pontuacao(game->matriz[k - 2][j]);
                        move_realizado = true;
                        break;
                    }
                    else if(game->matriz[k - 1][j] == '0')
                    {
                        game->matriz[k - 1][j] = game->matriz[k][j];
                        game->matriz[k][j] = '0';
                        move_realizado = true;
                        game->pontos++;
                    }
                    k--;
                }
            }
        }
    }
    
    if (move_realizado)
    {
        popula_matriz(game);
    }
}
void move_down(controle *game)
{
    bool move_realizado = false;

    for (int j = 0; j < COL; j++)
    {
        for (int i = LIN - 2; i >= 0; i--)
        {
            if (game->matriz[i][j] != '0')
            {
                int k = i;
                while (k < LIN - 1 && (game->matriz[k + 1][j] == '0' || game->matriz[k + 1][j] == game->matriz[k][j]))
                {
                    if (k + 2 < LIN && game->matriz[k + 2][j] == game->matriz[k + 1][j] && game->matriz[k + 1][j] == game->matriz[k][j])
                    {
                        game->matriz[k + 2][j] = game->matriz[k][j] + 1;
                        game->matriz[k + 1][j] = '0';
                        game->matriz[k][j] = '0';
                        game->pontos += calcula_pontuacao(game->matriz[k + 2][j]);
                        move_realizado = true;
                        break;
                    }
                    else if(game->matriz[k + 1][j] == '0')
                    {
                        game->matriz[k + 1][j] = game->matriz[k][j];
                        game->matriz[k][j] = '0';
                        move_realizado = true;
                        game->pontos++;
                    }
                    k++;
                }
            }
        }
    }
    
    if (move_realizado)
    {
        popula_matriz(game);
    }
}
void move_left(controle *game)
{
    bool move_realizado = false;

    for (int i = 0; i < LIN; i++)
    {
        for (int j = 1; j < COL; j++)
        {
            if (game->matriz[i][j] != '0')
            {
                int k = j;
                while (k > 0 && (game->matriz[i][k - 1] == '0' || game->matriz[i][k - 1] == game->matriz[i][k]))
                {
                    if (k - 2 >= 0 && game->matriz[i][k - 2] == game->matriz[i][k - 1] && game->matriz[i][k - 1] == game->matriz[i][k])
                    {
                        game->matriz[i][k - 2] = game->matriz[i][k] + 1;
                        game->matriz[i][k - 1] = '0';
                        game->matriz[i][k] = '0';
                        move_realizado = true;
                        game->pontos += calcula_pontuacao(game->matriz[i][k - 2]);
                        break;
                    }
                    else if(game->matriz[i][k - 1] == '0')            
                    {
                        game->matriz[i][k - 1] = game->matriz[i][k];
                        game->matriz[i][k] = '0';
                        move_realizado = true;
                        game->pontos++;
                    }
                    k--;
                }
            }
        }
    }
    
    if (move_realizado)
    {
        popula_matriz(game);
    }
}

void mover_pecas(controle *game)
{
    int direcao = tela_tecla();
    switch (direcao)
    {
        case c_right:
            move_right(game);
            break;
        case c_left:
            move_left(game);
            break;
        case c_up:
            move_up(game);
            break;
        case c_down:
            move_down(game);
            break;
        default:
            break;
    }
}

void get_nome(controle *game)
{
    int i = 0;
    char nome[TAM];
    
    while (true)
    {
        int tecla = tela_tecla();

        if (tecla == c_enter)
            break;
        if (tecla != c_none)
        {
            if (tecla == c_back)
            {
                if (i > 0)
                {
                    i--;
                    nome[i] = '\0';  
                }
            }
            else
            {
                if(i < TAM - 1)
                {
                    nome[i] = tecla;
                    nome[i+1] = '\0'; 
                    i++;
                }
            }          
            tela_atualiza();     
            tela_texto_esq(400, 400, 20, branco, "Insira seu nome:");
            tela_texto_dir(410, 400, 20, branco, nome);
        }
    }
    strncpy(game->nome, nome, TAM);
}


void salvar_pontuacao(controle *game)
{
    FILE *arq;
    arq = fopen("pontuacao.txt", "a");
    if (arq == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        exit(1);
    }
    get_nome(game);
    fprintf(arq, "%s: %d Pontos!!\n", game->nome, game->pontos);
    fclose(arq);
}

void fim_de_jogo(controle *game)
{
    int i, j;
    for (i = 0; i < LIN; i++)
    {
        for (j = 0; j < COL; j++)
        {
            if (game->matriz[i][j] == '0')
            {
                game->fim = false;
                return;
            }else if(game->matriz[i][j] == 'F')
            {
                tela_atualiza();
                tela_texto(TELA_X/2, TELA_Y/2 - 30, 30, branco, "Parabens voce ganhou!\n");
                game->fim = true;
                salvar_pontuacao(game);
                return;
            }
        }
    }
    tela_atualiza();
    tela_texto(TELA_X/2, TELA_Y/2 - 30, 30, branco, "Voce perdeu!\n");
    salvar_pontuacao(game);
    game->fim = true;
}
void inicia_jogo(controle *game)
{
    tela_inicio(TELA_X, TELA_Y,"Game");
    inicializa_matriz(game);
    game->pontos = 0;
} 
void roda_jogo(controle *game)
{

    imprime_na_tela(game);
    mover_pecas(game);
    fim_de_jogo(game);
    tela_atualiza();
}

int main()
{
    controle game;
    srand(time(NULL));
    inicia_jogo(&game);
    while (game.fim == false)
    {   
        roda_jogo(&game);
    }    
    tela_fim();
    return 0;
}
