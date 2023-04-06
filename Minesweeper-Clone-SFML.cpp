#include <iostream>
#include "GameManager.cpp"
using namespace std;

int main()
{
    GameManager *game = new GameManager();
    game->PlayGame();
    return 0;
}

