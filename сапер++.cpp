#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

using namespace sf;

// Функция открытия соседних ячеек
void openCells(int x, int y, int gridLogic[12][12], int gridView[12][12]) {
    if (x < 1 || x > 10 || y < 1 || y > 10) return; // Вне границ
    if (gridView[x][y] != 10) return;              // Уже открыто или помечено
    gridView[x][y] = gridLogic[x][y];              // Открываем ячейку

    if (gridLogic[x][y] == 0) { // Если пустая клетка
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx != 0 || dy != 0) { // Исключаем саму клетку
                    openCells(x + dx, y + dy, gridLogic, gridView);
                }
            }
        }
    }
}

// Функция обработки двойного клика
void doubleClickOpen(int x, int y, int gridLogic[12][12], int gridView[12][12]) {
    int flagCount = 0;

    // Подсчет количества флажков вокруг ячейки
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 1 && nx <= 10 && ny >= 1 && ny <= 10 && gridView[nx][ny] == 11) {
                flagCount++;
            }
        }
    }

    // Если количество флажков совпадает с числом в ячейке, открываем соседние клетки
    if (flagCount == gridLogic[x][y]) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 1 && nx <= 10 && ny >= 1 && ny <= 10 && gridView[nx][ny] == 10) {
                    openCells(nx, ny, gridLogic, gridView);
                }
            }
        }
    }
}

// Функция проверки победы
bool congratulations(int gridLogic[12][12], int gridView[12][12]) {
    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {
            if (gridLogic[i][j] == 9) { // Если мина
                if (gridView[i][j] != 11) return false; // Мина должна быть помечена
            } else {
                if (gridView[i][j] != gridLogic[i][j]) return false; // Остальные клетки должны быть открыты
            }
        }
    }
    return true; // Все условия выполнены, игрок выиграл
}

int main() {
    RenderWindow app(VideoMode(400, 400), "Minesweeper!");
    srand(time(0));

    int w = 32; // Размер клетки
    int gridLogic[12][12]; // Логическое поле (где мины и числа)
    int gridView[12][12];  // Видимое поле

    Texture t;
    t.loadFromFile("C:\\Users\\ASUS\\Desktop\\Дитя С++\\saper.jpg");
    Sprite s(t);

    // Инициализация полей
    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {
            gridView[i][j] = 10; // Все клетки закрыты
            if (rand() % 5 == 0) gridLogic[i][j] = 9; // Мина
            else gridLogic[i][j] = 0; // Пустая клетка
        }
    }

    // Подсчет чисел вокруг мин
    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {
            if (gridLogic[i][j] == 9) continue;
            int n = 0;
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = i + dx;
                    int ny = j + dy;
                    if (nx >= 1 && nx <= 10 && ny >= 1 && ny <= 10 && gridLogic[nx][ny] == 9) {
                        n++;
                    }
                }
            }
            gridLogic[i][j] = n;
        }
    }

    bool first_click = false;
    Clock clock;

    while (app.isOpen()) {
        Event e;
        Vector2i pos = Mouse::getPosition(app);
        int x = pos.x / w;
        int y = pos.y / w;

        while (app.pollEvent(e)) {
            if (e.type == Event::Closed)
                app.close();

            if (e.type == Event::MouseButtonPressed) {
                if (!first_click) { // Обработка первого клика
                    if (gridLogic[x][y] == 9) { // Если на первом клике мина, перемещаем её
                        gridLogic[x][y] = 0;
                        for (int i = 1; i <= 10; i++) {
                            for (int j = 1; j <= 10; j++) {
                                if (gridLogic[i][j] != 9) {
                                    gridLogic[i][j] = 9;
                                    break;
                                }
                            }
                        }
                    }
                    openCells(x, y, gridLogic, gridView);
                    first_click = true;
                }

                if (e.mouseButton.button == Mouse::Left) {
                    gridView[x][y] = gridLogic[x][y];
                    if (gridLogic[x][y] == 9) app.close(); // Клик по мине
                    else if (gridLogic[x][y] == 0) openCells(x, y, gridLogic, gridView); // Клик по пустой клетке
                } else if (e.mouseButton.button == Mouse::Right) { // Флажки
                    if (gridView[x][y] == 10) gridView[x][y] = 11;
                    else if (gridView[x][y] == 11) gridView[x][y] = 10;
                }

                // Проверка победы
                if (congratulations(gridLogic, gridView)) {
                    app.close();
                }
            }
        }

        // Отображение игры
        app.clear(Color::White);
        for (int i = 1; i <= 10; i++) {
            for (int j = 1; j <= 10; j++) {
                s.setTextureRect(IntRect(gridView[i][j] * w, 0, w, w));
                s.setPosition(i * w, j * w);
                app.draw(s);
            }
        }
        app.display();
    }

    return 0;
}
