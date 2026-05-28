#include <SFML/Graphics.hpp>
#include "Game.h"
/*
DZIALANIE GRY W ZAMYSLE
1. Tworzymy okno gry
2. Tworzymy gracza
3. Wchodzimy do pętli gry
4. Sprawdzamy klawisze
5. Aktualizujemy pozycję gracza
6. Czyścimy ekran
7. Rysujemy gracza
8. Pokazujemy nową klatkę
9. Powtarzamy wszystko od nowa
*/
int main()
{
    Game game;
    game.run();

    return 0;
}

