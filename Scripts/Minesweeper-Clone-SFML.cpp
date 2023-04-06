#include <iostream>
#include "GameManager.cpp"
using namespace std;

int main()
{
    // Gameloop obj.play() Method
    GameManager *game = new GameManager();
    game->PlayGame();
    return 0;
}

