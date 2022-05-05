Nomes: João Vítor Schimuneck de Souza e João Pedro Licks Corso

BattleINF :)

Relativo aos structs:

JOGADOR: tem todas as informações do jogador, como posição, velocidade, vidas, score e o retângulo
para calcular colisões

INIMIGOS: tem todas as informações dos inimigos, como posição, retângulo para colisões, se o inimigo está ativo

TIJOLOS: informações sobre os tijolos do mapa, posição, ativos ou não, retângulo para colisões

CELULA: informações sobre a célula de energia que o jogador pode pegar para boostar seu personagem

HIGHSCORES: relativo às maiores pontuações dos jogadores (nome e pontuação)

O jogo começa sem nenhum inimigo na tela. Eles spawnam a cada 5 segundos e, para passar de nivel, o jogador deve eliminar
todos os inimigos da tela, sendo que o jogador deve também ter ficado, no mínimo, 10 segundos no nível.

O array bidimimensional TIJOLOS tij armazena os dados do arquivo txt lido como fase
JOGADOR player é o jogador
PROJETIL bullets é o array dos tiros do jogador
PROJETIL enemy_bullets é o array dos tiros dos inimigos
CELULA energia é a célula que aparece aleatoriamente
INIMIGOS enemy é o array dos inimigos
HIGHSCORES ranking tem os scores dos melhores jogadores
 