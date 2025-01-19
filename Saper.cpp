#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;

void openCells(int x, int y, int gridLogic[12][12], int gridView[12][12]) {
    if (x < 1 || x > 10 || y < 1 || y > 10) return; // Вне границ
    if (gridView[x][y] != 10) return; // Уже открыто или помечено
    gridView[x][y] = gridLogic[x][y];

    if (gridLogic[x][y] == 0) {
        openCells(x + 1, y, gridLogic, gridView);
        openCells(x - 1, y, gridLogic, gridView);
        openCells(x, y + 1, gridLogic, gridView);
        openCells(x, y - 1, gridLogic, gridView);
        openCells(x + 1, y + 1, gridLogic, gridView);
        openCells(x - 1, y - 1, gridLogic, gridView);
        openCells(x + 1, y - 1, gridLogic, gridView);
        openCells(x - 1, y + 1, gridLogic, gridView);
    }
}

void doubleClickOpen(int x, int y, int gridLogic[12][12], int gridView[12][12]) {
    int flagCount = 0;
    if (gridLogic[x][y] == 0) {
        openCells(x,y, gridLogic, gridView);
    }
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

    // Если количество флажков совпадает с числом в ячейке, открываем все соседние клетки
    if (flagCount == gridLogic[x][y]) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 1 && nx <= 10 && ny >= 1 && ny <= 10 && gridView[nx][ny] == 10) {
                    gridView[nx][ny] = gridLogic[nx][ny];
                    if (gridLogic[nx][ny] == 0) {
                        openCells(nx, ny, gridLogic, gridView);
                    }
                }
            }
        }
    }
}
int main()
{
    RenderWindow app(VideoMode(400, 400), "Minesweeper!");
    srand(time(0));
    int w = 32;
    int gridLogic[12][12];
    int gridView[12][12];
    Texture t;
    t.loadFromFile("C:\\Users\\ASUS\\Desktop\\Дитя С++\\saper.jpg");
    Sprite s(t);

    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 10; j++)
        {
            gridView[i][j] = 10;
            if (i == 5 && j == 5) continue;
            if (rand() % 5 == 0) gridLogic[i][j] = 9;
            else gridLogic[i][j] = 0;
        }
    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 10; j++)
        {
            int n = 0;
            if (gridLogic[i][j] == 9) continue;
            if (i + 1 <= 10 && gridLogic[i + 1][j] == 9) n++;
            if (j + 1 <= 10 && gridLogic[i][j + 1] == 9) n++;
            if (i - 1 >= 1 && gridLogic[i - 1][j] == 9) n++;
            if (j - 1 >= 1 && gridLogic[i][j - 1] == 9) n++;
            if (i + 1 <= 10 && j + 1 <= 10 && gridLogic[i + 1][j + 1] == 9) n++;
            if (i - 1 >= 1 && j - 1 >= 1 && gridLogic[i - 1][j - 1] == 9) n++;
            if (i - 1 >= 1 && j + 1 <= 10 && gridLogic[i - 1][j + 1] == 9) n++;
            if (i + 1 <= 10 && j - 1 >= 1 && gridLogic[i + 1][j - 1] == 9) n++;
            gridLogic[i][j] = n;
        }
    bool first_click = false;
    bool double_click = false;
    Clock clock;

    while (app.isOpen())
    {
        Event e;
        Vector2i pos = Mouse::getPosition(app);
        int x = pos.x / w;
        int y = pos.y / w;

        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();

            if (e.type == Event::MouseButtonPressed)
            {
                if (first_click == false) {
                    // Открываем область вокруг первого нажатия
                    for (int dx = -1; dx <= 1; dx++) {
                        for (int dy = -1; dy <= 1; dy++) {
                            int nx = x + dx;
                            int ny = y + dy;
                            if (nx >= 1 && nx <= 10 && ny >= 1 && ny <= 10 && gridLogic[nx][ny] != 9) {
                                gridView[nx][ny] = gridLogic[nx][ny];
                                if (gridLogic[nx][ny] == 0) {
                                    openCells(nx, ny, gridLogic, gridView);
                                }
                            }
                        }
                    }
                    first_click = true;
                }
                if (e.mouseButton.button == Mouse::Left) {
                    if (clock.getElapsedTime().asMilliseconds() < 300) {
                        double_click = true;
                    }
                    else {
                        double_click = false;
                    }
                    clock.restart();

                    if (double_click) {
                        if (gridView[x][y] > 0 && gridView[x][y] < 9) {
                            doubleClickOpen(x, y, gridLogic, gridView);
                        }
                    }
                    else {
                        gridView[x][y] = gridLogic[x][y];
                        if (gridView[x][y] == 9) app.close();
                    }
                }
                else if (e.mouseButton.button == Mouse::Right) {
                    if (gridView[x][y] == 10) gridView[x][y] = 11;
                    else if (gridView[x][y] == 11) gridView[x][y] = 10;
                }
            }
        }

        app.clear(Color::White);

        for (int i = 1; i <= 10; i++)
            for (int j = 1; j <= 10; j++)
            {
                s.setTextureRect(IntRect(gridView[i][j] * w, 0, w, w));
                s.setPosition(i * w, j * w);
                app.draw(s);
            }

        app.display();
    }

    return 0;
}

