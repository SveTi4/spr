#include "Sapper.h"

#include <SFML/Graphics.hpp>

// Конструкторы класса MWindow
MWindow::MWindow(int Width, int Height) {
    width = Width;
    height = Height;
}
MWindow::MWindow() {
    width = 500;
    height = 500;
}
// Функции получения расширения окна
int MWindow::GetWidth() { return width; }
int MWindow::GetHeight() { return height; }
// Конструктор класса Field

Position2D::Position2D (): x(0), y(0) {}
void Position2D::setX(int X) {
    x = X;
}
void Position2D::setY(int Y) {
    y = Y;
}
int Position2D::getX() {
    return x;
}
int Position2D::getY() {
    return y;
}

Cell::Cell (int NewCellSize, sf::RenderWindow &window): 
    cellScale((float)(NewCellSize) / (float)cellSize),
    newCellSize(NewCellSize), 
    state(10),
    window(&window)
{
    setX(0);
    setY(0);
}
void Cell::setState(int State) {
    state = State;
}
int Cell::getState() {
    return state;
}
void Cell::draw() {
    // Создание тестур и спрайта клетки
    cellTexture.loadFromFile("src/textures/cells.png");
    sf::Sprite spriteCell(cellTexture);
    
    spriteCell.setTextureRect(
        sf::IntRect(state * cellSize, 0, cellSize, cellSize));  // Выделение необходимой части спрайта
    spriteCell.setPosition(getX(), getY());  // Вычисление позиции отрисовки
    spriteCell.setScale(cellScale, cellScale);  // Увеличение спрайта до необходимого размера
    window->draw(spriteCell);  // Отрисовка
};

Field::Field(sf::RenderWindow &window): window(window) {
    cell = new Cell(60, window);
    cell->setState(10);
    setField();
}
void Sapper::Restart() {
    field->setField();
    gameStatus = 1;
    first_step = 1;
}
int Field::getCell(int x, int y) {
    return LogicF[x][y];
}
void Field::setCell(int x, int y, int data) {
    LogicF[x][y] = data;
}
int Field::getViewCell(int x, int y) {
    return ViewF[x][y];
}
void Field::setViewCell(int x, int y, int data) {
    ViewF[x][y] = data;
}
// Открытие ячейки
void Field::openCell(int x, int y) {
    ViewF[x][y] = LogicF[x][y];
}
// Открытие пустых ячеек вокруг открытой
void Field::emptyOpen(int x, int y) {
    for (int i = x - 1; i <= x + 1; i++)
        for (int j = y - 1; j <= y + 1; j++)
            if (LogicF[i][j] != 0 && i > 0 && j > 0 && i < size + 2 &&
                j < size + 2)
                ViewF[i][j] = LogicF[i][j];  // открытие
}
// Рекурсивное открытие вокруг выбранной ячейки
void Field::open(int x, int y) {
    emptyOpen(x, y);
    for (int i = x - 1; i <= x + 1; i++)
        for (int j = y - 1; j <= y + 1; j++) {
            if (LogicF[i][j] == 0 && ViewF[i][j] != 0 && i > 0 && j > 0 &&
                i < size + 2 && j < size + 2) {
                ViewF[i][j] = LogicF[i][j];
                open(i, j);
            }
        }
}
// Первое открытие
void Field::firstOpen(int x, int y) {
    openCell(y, x);  // Открытие выбранной ячейки
    addBomns(y, x);  // Заполнение поля минами и цифрами
    open(y, x);  // Рекурсивное открытие близлежащих пустых ячеек
}
void Field::setField() {
    for (int i = 0; i < size + 2; i++)
        for (int j = 0; j < size + 2; j++) {
            LogicF[i][j] = 0;
            ViewF[i][j] = 10;
        }
}
// Увеличение численных значений вокруг мины
void Field::addNumber(int x, int y) {
    int ii, jj;
    for (ii = x - 1; ii <= x + 1; ii++)
        for (jj = y - 1; jj <= y + 1; jj++)
            if (LogicF[ii][jj] < 8) LogicF[ii][jj]++;
}
// Расстановка бомб
void Field::addBomns(int x, int y) {
    int i, j;
    for (i = 1; i < size + 2; i++)
        for (j = 1; j < size + 2; j++) {
            if (!(i == x && j == y) && !(i == x - 1 && j == y - 1) &&
                !(i == x + 1 && j == y + 1) && !(i == x - 1 && j == y + 1) &&
                !(i == x + 1 && j == y - 1) && !(i == x && j == y + 1) &&
                !(i == x && j == y - 1) && !(i == x - 1 && j == y) &&
                !(i == x + 1 && j == y))
                if (rand() % 4 == 0 && i > 0 && j > 0 && i < size + 1 &&
                    j < size + 1) {
                    LogicF[i][j] = 9;
                    ViewF[i][j] = 11;
                    addNumber(i, j);  // Добавление увеличение цифр вокруг мины
                }
        }
}
void Field::fieldDraw() {
    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {
            cell->setX((i-1) * 60);
            cell->setY((j-1) * 60);
            cell->setState(ViewF[i][j]);
            cell->draw();
        }   
    }
}
Gameover::Gameover(sf::RenderWindow &window): window(window) {}
// Создание затемнённого фона при окончании игры
void Gameover::GameOverBG() {
    sf::Texture GOtexture;
    GOtexture.loadFromFile("src/textures/bg.png");
    sf::Sprite GOsprite(GOtexture);
    GOsprite.setPosition(0, 0);
    GOsprite.setScale(1, 1);
    window.draw(GOsprite);
    SetMousePos();  // Проверка положения мыши для кнопки рестарта, описанной ниже
}
// Отрисовка кнопки рестарта игры
void Gameover::DrawRestart() {
    sf::Texture restartT;
    restartT.loadFromFile("src/textures/Restart.png");
    sf::Sprite restartS(restartT);
    restartS.setPosition(130, 300);
    restartS.scale(1, 1);
    window.draw(restartS);
}
// Отрисовка надписи и рестарта при проигрыше
void Gameover::DrawLose() {
    GameOverBG();
    sf::Texture LoseT;
    LoseT.loadFromFile("src/textures/lose.png");
    sf::Sprite LoseS(LoseT);
    LoseS.setPosition(60, 0);
    LoseS.scale(0.8, 0.8);
    window.draw(LoseS);
    DrawRestart();
}
// Отрисовка надписи и рестарта при выигрыше
void Gameover::DrawWin() {
    GameOverBG();
    sf::Texture WinT;
    WinT.loadFromFile("src/textures/win.png");
    sf::Sprite WinS(WinT);
    WinS.setPosition(60, 0);
    WinS.scale(0.8, 0.8);
    window.draw(WinS);
    DrawRestart();
}
// Проврка наведения курсора на кнопку рестарта
void Gameover::SetMousePos() {
    sf::Vector2i pos = sf::Mouse::getPosition(window);
    if (pos.x > 130 && pos.x < 430 && pos.y > 300 && pos.y < 400) {
        CheckButton = 1;
    } else
        CheckButton = 0;
}
// Если нажата ЛКМ
int Gameover::EventML() {
    // если курсор на кнопке то вернёт 1
    if (CheckButton) return 1;
    return 0;
}

Sapper::Sapper(sf::RenderWindow &window): window(window){
    field = new Field(window);
};
// Проверка наведён ли курсор мыши на поле
int Sapper::CheckActiveField() {
    MousePos.x = int(float(sf::Mouse::getPosition(window).x)/60 + 1);
    MousePos.y = int(float(sf::Mouse::getPosition(window).y)/60 + 1);
    if (MousePos.x * 60 <= 600 && MousePos.x >= 0 &&
        MousePos.y * 60 <= 600 && MousePos.y >= 0)
        return 1;
    return 0;
}
void Sapper::EventML() {
    if (CheckActiveField() && gameStatus == 1)  // если мышь на игровом поле то
    {
        // условие первого открытия
        if (first_step == 1) {
            field->firstOpen(MousePos.y, MousePos.x);
            first_step = 0;
        }
        field->openCell(MousePos.x, MousePos.y);  // Открытие указанной ячейки
        // Если открытая ячейка пустая то
        // Эта и рядом лежащие пустые ячейки открываются
        if (field->getCell(MousePos.x, MousePos.y) == 0 && gameStatus == 1) {
            field->open(MousePos.x, MousePos.y);
        }
        // Если открытая ячейка с миной то
        if (field->getCell(MousePos.x, MousePos.y) == 9 && gameStatus == 1) {
            gameStatus = 0;  // Статус игры меняется на проигрышный
            // Вскрываются все остальные ячейки
            for (int i = 1; i < 10 + 1; i++)
                for (int j = 1; j < 10 + 1; j++) field->openCell(i, j);;
        }
        // Проверка на победу
        if (field->checkWin() == 2 && gameStatus == 1) {
            gameStatus = 2;  // Статус игры меняется на выигрышный
        }
    } else if (CheckActiveField() && gameStatus != 1) {
        gameover = new Gameover(window);
        gameover->SetMousePos();
        if(gameover->EventML() == 1)
            Restart();
    }
}
void Sapper::EventMR() {
    if (CheckActiveField() && gameStatus == 1) {
        // Если это не первое открытие и игра не окончена, то
        if (first_step != 1 && gameStatus == 1) {
            if (field->getViewCell(MousePos.x, MousePos.y) == 10)
                field->setViewCell(MousePos.x, MousePos.y, 11); // Ставится флажок
            else if (field->getViewCell(MousePos.x, MousePos.y) == 11)
                field->setViewCell(MousePos.x, MousePos.y, 10); // Убирается флажок
        }
    }
}
void Sapper::drawGame() {
    field->fieldDraw();
}
void Sapper::drawGameeOver() {
    gameover = new Gameover(window);
    gameover->DrawLose();
}
void Sapper::drawGameWin() {
    gameover = new Gameover(window);
    gameover->DrawWin();
}
// проверка на победу
int Field::checkWin() {
    for (int i = 1; i < size + 1; i++)
        for (int j = 1; j < size + 1; j++)
            if (ViewF[i][j] == 10) return 1;
    return 2;
}