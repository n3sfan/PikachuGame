enum ScreenState {
    MENU,
    GAME,
    LEADERBOARD,
    MENULEVEL,
    MENUSIZE
};
enum ScreenCurrent{
    SCREEN_OF_GAME,
    SCREEN_OF_LEADERBOARD
};

extern ScreenState currentScreen;
extern ScreenCurrent Screen;

void Menu();
void MenuLevel();