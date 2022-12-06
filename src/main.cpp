#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Sapper.h"

int main() {
    srand(time(NULL));
    MWindow Sapper_window(600, 600);  // Задаём размеры окна через класс MWindow
    // Отрисовка окна
    sf::RenderWindow window(
        sf::VideoMode(Sapper_window.GetWidth(), Sapper_window.GetHeight()),
        "Sapper", Style::Close);

    Game *sapper = new Sapper(window);
    
    sapper->drawGameeOver();
    sapper->drawGameWin();

    while (window.isOpen()) {
        // Заливка фона окна
        window.clear(Color(105, 105, 105));
        // Отрисовка игры
        sapper->drawGame();
        if (sapper->gameStatus == 0)
            sapper->drawGameeOver();
        else if (sapper->gameStatus == 2)
            sapper->drawGameWin();
        // Обработка нажатий
        sf::Event event;
        while (window.pollEvent(event)) {
            // Закрытие окна
            if (event.type == sf::Event::Closed) window.close();
            // Нажатие кнопок мыши
            if (event.type == Event::MouseButtonPressed) {
                // если нажата ЛКМ
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Проверка на рестарт
                    sapper->EventML();
                // если нажата ПКМ
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    sapper->EventMR();
                }
            }
        }
        window.display();  // отрисовка окна
    }
    return 0;
}