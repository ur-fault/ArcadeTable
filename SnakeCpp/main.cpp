#include <iostream>
#include <tuple>
#include <vector>

class Snake
{
public:
    Snake(int x, int y)
    {
        body.push_back(std::make_tuple(x, y));
    }

    void move(int foodX, int foodY)
    {
    }

    std::tuple<int, int> get_head()
    {
        return body[0];
    }

    bool is_head(int x, int y)
    {
        return get_head() == std::make_tuple(x, y);
    }

private:
    std::vector<std::tuple<int, int>> body;
};

class Game
{
public:
    Game(int width, int height)
    {
        this->width = width;
        this->height = height;
        p1 = new Snake(0, 0);
        p2 = new Snake(width - 1, height - 1);
    }

    void draw() {
        for (int i = 0; i < width; i++) {

        }
    }

private:
    int width, height;
    Snake *p1;
    Snake *p2;
};

int main()
{
    Game *game = new Game(10, 10);
    game->draw();
    return 0;
}