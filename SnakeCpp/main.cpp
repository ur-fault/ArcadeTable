#include <iostream>
#include <algorithm>
#include <tuple>
#include <vector>
#include <optional>
#include <random>
#include <thread>

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

typedef std::tuple<int, int> Point;

Point direction_offset(Direction direction)
{
    switch (direction)
    {
    case UP:
        return std::make_tuple(0, -1);
    case DOWN:
        return std::make_tuple(0, 1);
    case LEFT:
        return std::make_tuple(-1, 0);
    case RIGHT:
        return std::make_tuple(1, 0);
    default:
        exit(1);
    }
}

int get_x(Point tuple)
{
    return std::get<0>(tuple);
}

int get_y(Point tuple)
{
    return std::get<1>(tuple);
}

Point add_tuple(Point tuple1, Point tuple2)
{
    return std::make_tuple(get_x(tuple1) + get_x(tuple2), get_y(tuple1) + get_y(tuple2));
}

struct GameState
{
    int width;
    int height;
    std::tuple<int, int> food;
};

class Snake
{
public:
    Snake(int x, int y)
    {
        body.push_back(std::make_tuple(x, y));
        direction = x == 0 ? RIGHT : LEFT;
    }

    bool update(GameState state)
    {
        body.insert(body.begin(), add_tuple(get_head(), direction_offset(direction)));

        if (get_head() == state.food)
            return true;

        body.pop_back();
        return false;
    }

    void set_direction(Direction direction)
    {
        this->direction = direction;
    }

    Point get_head()
    {
        return body[0];
    }

    bool is_head(int x, int y)
    {
        return get_head() == std::make_tuple(x, y);
    }

    bool body_contains(int x, int y)
    {
        return std::find(body.begin(), body.end(), std::make_tuple(x, y)) != body.end();
    }

    int get_score()
    {
        return body.size();
    }

private:
    std::vector<Point> body;
    Direction direction;
};

class Game
{
public:
    std::chrono::duration<int> delay;

    Game(int width, int height, std::chrono::duration<int> delay = std::chrono::seconds(1))
    {
        this->width = width;
        this->height = height;
        p1 = new Snake(0, 0);
        p2 = new Snake(width - 1, height - 1);

        auto food = get_random_food();
        foodX = get_x(food);
        foodY = get_y(food);

        this->delay = delay;
    }

    Point get_random_food()
    {
        return std::make_tuple(std::rand() % width, std::rand() % height);
    }

    Point get_food()
    {
        return std::make_tuple(foodX, foodY);
    }

    GameState get_state()
    {
        return GameState{
            width,
            height,
            get_food(),
        };
    }

    std::optional<int> update()
    {
        auto state = get_state();
        p1->update(state);
        p2->update(state);

        return std::nullopt;
    }

    void draw()
    {
        std::cout << "P1: " << get_x(p1->get_head()) << ":" << get_y(p1->get_head()) << std::endl;
        std::cout << "p2: " << get_x(p2->get_head()) << ":" << get_y(p2->get_head()) << std::endl;
        std::cout << '+' << std::string(width * 2, '-') << '+' << std::endl;

        for (int j = 0; j < width; j++)
        {
            std::cout << '|';
            for (int i = 0; i < height; i++)
            {
                if (p1->is_head(i, j))
                    std::cout << "AA";
                else if (p2->is_head(i, j))
                    std::cout << "BB";
                else if (p1->body_contains(i, j))
                    std::cout << "aa";
                else if (p2->body_contains(i, j))
                    std::cout << "bb";
                else if (i == foodX && j == foodY)
                    std::cout << "XX";
                else
                    std::cout << "  ";
            }

            std::cout << '|' << std::endl;
        }

        std::cout << '+' << std::string(width * 2, '-') << '+' << std::endl;
    }

    int game_loop()
    {
        while (true)
        {
            draw();
            std::this_thread::sleep_for(delay);
            update();
        }
    }

private:
    int width, height;
    Snake *p1;
    Snake *p2;
    int foodX, foodY;
};

const bool ALLOW_CROSS = true;

int main()
{
    Game *game = new Game(10, 10);
    game->game_loop();

    return 0;
}