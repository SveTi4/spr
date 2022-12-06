#ifndef SAPPER_H
#define SAPPER_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib>
#include <time.h>


using namespace sf;

class Position2D {
    private:
        int x;
        int y;
    public:
        Position2D();
        void setX(int X);
        void setY(int Y);
        int getX();
        int getY();
};
// класс задающий размеры окна
class MWindow
{
    private:
        int width;
        int height;
    public:
        // конструкторы класса MWindow
        MWindow();
        MWindow(int Width, int Height);
        // функции получения расширения окна
        int GetWidth ();
        int GetHeight ();
};

class Cell: public Position2D {
    private:
        int cellSize = 100;
        int newCellSize;
        int state;
        
        float cellScale;
        
        sf::Texture cellTexture;
        sf::RenderWindow *window;
    public:
        Cell(int NewCellSize, sf::RenderWindow &window);
        void setState(int State);
        int getState();
        void draw();
};
class Field {
    private:
        sf::RenderWindow &window;
        Cell *cell;
        int size = 10;
        int ViewF[12][12]; // Визуальный массив
        int LogicF[12][12]; // Логический массив

        void addBomns(int x, int y);
        void addNumber(int x, int y);
        void emptyOpen(int x, int y);
        
        
    public:
        Field(sf::RenderWindow &window);
        void setCell(int x, int y, int data);
        int getCell(int x, int y);
        void setViewCell(int x, int y, int data);
        int getViewCell(int x, int y);
        void fieldDraw();
        void setField();
        int checkWin();
        void open(int x, int y);
        void firstOpen(int x, int y);
        void openCell(int x, int y);
        
};

// Класс отвечающий за итрисовку после окончания игры
class Gameover
{
    private:
        sf::RenderWindow &window;
        int CheckButton = 0; // стутс наведения мыши на кноаку рестарта
        // Создание затемнённого фона при окончании игры
        void GameOverBG ();
        // Отрисовка кнопки рестарта игры
        void DrawRestart();
    public:
        Gameover(sf::RenderWindow &window);
        // Отрисовка надписи и рестарта при проигрыше
        void DrawLose ();
        // Отрисовка надписи и рестарта при выигрыше
        void DrawWin ();
        // Проврка наведения курсора на кнопку рестарта
        void SetMousePos();
        int EventML(); // Если нажата ЛКМ
};
class Game {
    public:
        Game() {};
        int gameStatus = 1;
        virtual void Restart() = 0;
        virtual void EventML() = 0;
        virtual void EventMR() = 0;
        virtual void drawGame() = 0;
        virtual void drawGameeOver() = 0;
        virtual void drawGameWin() = 0;
};
class Sapper: public Game {
    private:
        sf::RenderWindow &window;
        sf::Vector2i MousePos;
        Field *field;
        Gameover *gameover;
        int first_step = 1;

        int CheckActiveField();
    public:
        Sapper();
        Sapper(sf::RenderWindow &window);
        void Restart();
        void EventML();
        void EventMR();
        void drawGame();
        void drawGameeOver();
        void drawGameWin();
};

#endif
