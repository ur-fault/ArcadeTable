#include <algorithm>
#include <iostream>
#include <optional>
#include <random>
#include <thread>
#include <tuple>
#include <vector>

#include <curses.h>

enum Direction { UP, DOWN, LEFT, RIGHT };

typedef std::tuple<int, int> Point;

Point direction_offset(Direction direction) {
    switch (direction) {
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

int get_x(Point tuple) { return std::get<0>(tuple); }

int get_y(Point tuple) { return std::get<1>(tuple); }

Point add_points(Point tuple1, Point tuple2) {
    return std::make_tuple(get_x(tuple1) + get_x(tuple2),
                           get_y(tuple1) + get_y(tuple2));
}

struct GameState {
    int width;
    int height;
    std::tuple<int, int> food;
};

class Snake {
  public:
    Snake(int x, int y) {
        body.push_back(std::make_tuple(x, y));
        direction = x == 0 ? RIGHT : LEFT;
    }

    bool update(const GameState &state) {
        body.insert(body.begin(),
                    add_points(get_head(), direction_offset(direction)));

        if (get_head() == state.food)
            return true;

        body.pop_back();
        return false;
    }

    bool is_dead(GameState &state, Snake &other) {
        auto head = get_head();
        auto x = get_x(head), y = get_y(head);

        if (x < 0 || x >= state.width || y < 0 || y >= state.height ||
            body_contains(x, y) || other.snake_contains(x, y)) {
            return true;
        }

        return false;
    }

    void set_direction(Direction direction) { this->direction = direction; }

    Point get_head() { return body[0]; }

    bool is_head(int x, int y) { return get_head() == std::make_tuple(x, y); }

    const bool snake_contains(int x, int y) {
        return std::find(body.begin(), body.end(), std::make_tuple(x, y)) !=
               body.end();
    }

    const bool body_contains(int x, int y) {
        return std::find(body.begin() + 1, body.end(), std::make_tuple(x, y)) !=
               body.end();
    }

    int get_score() { return body.size(); }

  private:
    std::vector<Point> body;
    Direction direction;
};

class Game {
  public:
    std::chrono::duration<int> delay;

    Game(int width, int height,
         std::chrono::duration<int> delay = std::chrono::seconds(1)) {
        this->width = width;
        this->height = height;
        p1 = new Snake(0, 0);
        p2 = new Snake(width - 1, height - 1);
        // p2 = new Snake(width - 1, 0);

        auto food = get_random_food();
        foodX = get_x(food);
        foodY = get_y(food);

        this->delay = delay;
    }

    ~Game() {
        delete p1;
        delete p2;
    }

    Point get_random_food() {
        return std::make_tuple(std::rand() % width, std::rand() % height);
    }

    Point get_food() { return std::make_tuple(foodX, foodY); }

    GameState get_state() {
        return GameState{
            width,
            height,
            get_food(),
        };
    }

    int update(bool move) {
        int ch = getch();

        if (ch != ERR) {
            switch (ch) {
            case 'w':
                p1->set_direction(UP);
                break;
            case 's':
                p1->set_direction(DOWN);
                break;
            case 'a':
                p1->set_direction(LEFT);
                break;
            case 'd':
                p1->set_direction(RIGHT);
                break;
            }

            switch (ch) {
            case 'i':
                p2->set_direction(UP);
                break;
            case 'k':
                p2->set_direction(DOWN);
                break;
            case 'j':
                p2->set_direction(LEFT);
                break;
            case 'l':
                p2->set_direction(RIGHT);
                break;
            }

            switch (ch) {
            case KEY_BACKSPACE:
                return -1;
                break;
            }
        }

        if (move) {
            auto state = get_state();
            auto p1_ate = p1->update(state);
            auto p2_ate = p2->update(state);

            if (p1_ate || p2_ate) {
                auto food = get_random_food();
                foodX = get_x(food);
                foodY = get_y(food);
            }

            auto p1_dead = p1->is_dead(state, *p2);
            auto p2_dead = p2->is_dead(state, *p1);

            if (p1_dead && p2_dead) {
                return 1;
            } else if (p1_dead) {
                return 2;
            } else if (p2_dead) {
                return 3;
            }
        }

        return 0;
    }

    void draw() {
        wmove(stdscr, 0, 0);

        printw("P1: %d\n", p1->get_score());
        printw("P2: %d\n", p2->get_score());

        for (auto j = 0; j < height; j++) {
            for (auto i = 0; i < width; i++) {
                if (p1->is_head(i, j)) {
                    printw("AA");
                } else if (p2->is_head(i, j)) {
                    printw("BB");
                } else if (p1->snake_contains(i, j)) {
                    printw("aa");
                } else if (p2->snake_contains(i, j)) {
                    printw("bb");
                } else if (i == foodX && j == foodY) {
                    printw("XX");
                } else {
                    printw("..");
                }
            }
            printw("\n");
        }

        wrefresh(stdscr);
    }

    int game_loop() {
        auto last_move = std::chrono::system_clock::now();
        while (true) {
            auto now = std::chrono::system_clock::now();
            bool move = false;
            if (now - last_move > std::chrono::milliseconds(300)) {
                last_move = now;
                move = true;
            }

            auto update_res = update(move);
            if (update_res != 0) {
                return update_res + 1;
            }

            draw();
        }
    }

  private:
    int width, height;
    Snake *p1;
    Snake *p2;
    int foodX, foodY;
};

int main() {
    // init screan
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, true);
    keypad(stdscr, true);

    Game *game = new Game(10, 10);
    auto game_result = game->game_loop();

    endwin();

    if (game_result == 2) {
        std::cout << "Draw" << std::endl;
    } else if (game_result == 3) {
        std::cout << "Player 2 wins" << std::endl;
    } else if (game_result == 4) {
        std::cout << "Player 1 wins" << std::endl;
    }

    return 0;
}
