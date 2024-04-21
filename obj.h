// --------- ARQUIVOS OBJETOS ------

enum IDS{JOGADOR, MISSIL, FUNDO, INIMIGOS};

struct Nave
{
    int ID;
    int x;
    int y;
    int vidas;
    int velocidade;
    int borda_x;
    int borda_y;
    int pontos;

};

struct Missil
{
    int ID;
    int x;
    int y;
    int velocidade;
    bool ativo;
};

struct Fundo
{
    int ID;
    int x;
    int y;
    int velocidade;
};
