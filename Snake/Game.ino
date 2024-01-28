#include <microTuple.h>

const int BOARD_SIZE = 10 * 10;

typedef MicroTuple<int, int> Point;
typedef unsigned long time_t;

enum Direction { UP, DOWN, LEFT, RIGHT };

Point direction_offset(Direction direction) {
    switch (direction) {
    case UP:
        return Point(0, -1);
    case DOWN:
        return Point(0, 1);
    case LEFT:
        return Point(-1, 0);
    case RIGHT:
        return Point(1, 0);
    default:
        exit(1);
    }
}

Point operator+(Point a, Point b) {
    return Point(a.get<0>() + b.get<0>(), a.get<1>() + b.get<1>());
}

bool operator==(Point a, Point b) {
    return a.get<0>() == b.get<0>() && a.get<1>() == b.get<1>();
}

struct GameState {
    Point dims;
    Point food;
};

class Body {
  public:
    Body() { len = 0; }

    void push(Point x) { data[len++] = x; }

    Point pop() { return data[--len]; }

    void insert(int i, Point x) {
        for (int j = len; j > i; j--) {
            data[j] = data[j - 1];
        }
        data[i] = x;
        len++;
    }

    Point get(int i) { return data[i]; }

    Point &operator[](int i) { return data[i]; }

    int size() { return len; }

  private:
    int len;

    Point data[BOARD_SIZE];
};

class Snake {
  public:
    Snake(Point head) {
        body.push(head);
        direction = head.get<0>() == 0 ? RIGHT : LEFT;
    }

    Snake(int x, int y) : Snake(Point(x, y)) {}

    bool update(const GameState &state) {
        body.insert(0, get_head() + direction_offset(direction));

        if (get_head() == state.food)
            return true;

        body.pop();
        return false;
    }

    bool is_dead(GameState &state, Snake &other) {
        auto head = get_head();

        auto x = head.get<0>();
        auto y = head.get<1>();

        auto width = state.dims.get<0>();
        auto height = state.dims.get<1>();

        if (x < 0 || x >= width || y < 0 || y >= height ||
            body_contains(x, y) || other.snake_contains(x, y)) {
            return true;
        }

        return false;
    }

    void set_direction(Direction direction) { this->direction = direction; }

    Point get_head() { return body[0]; }

    bool is_head(int x, int y) { return get_head() == Point(x, y); }

    const bool snake_contains(int x, int y) {
        for (int i = 0; i < body.size(); i++) {
            if (body[i] == Point(x, y)) {
                return true;
            }
        }

        return false;
    }

    const bool body_contains(int x, int y) {
        for (int i = 1; i < body.size(); i++) {
            if (body[i] == Point(x, y)) {
                return true;
            }
        }

        return false;
    }

    int get_score() { return body.size(); }

  private:
    Body body;
    Direction direction;
};

class Game {
  public:
    time_t delay;

    Game(Point dims, time_t delay) {
        this->dims = dims;
        p1 = new Snake(0, 0);
        p2 = new Snake(dims);

        auto food = get_random_food();

        this->delay = delay;
    }

    ~Game() {
        delete p1;
        delete p2;
    }

    Point get_random_food() {
        return Point(random(dims.get<0>()), random(dims.get<1>()));
    }

    Point get_food() { return food; }

    GameState get_state() {
        return GameState{
            .dims = Point(dims),
            .food = get_food(),
        };
    }

    int update(bool move) {
        // int ch = getch();
        //
        // if (ch != ERR) {
        //     switch (ch) {
        //     case 'w':
        //         p1->set_direction(UP);
        //         break;
        //     case 's':
        //         p1->set_direction(DOWN);
        //         break;
        //     case 'a':
        //         p1->set_direction(LEFT);
        //         break;
        //     case 'd':
        //         p1->set_direction(RIGHT);
        //         break;
        //     }
        //
        //     switch (ch) {
        //     case 'i':
        //         p2->set_direction(UP);
        //         break;
        //     case 'k':
        //         p2->set_direction(DOWN);
        //         break;
        //     case 'j':
        //         p2->set_direction(LEFT);
        //         break;
        //     case 'l':
        //         p2->set_direction(RIGHT);
        //         break;
        //     }
        //
        //     switch (ch) {
        //     case KEY_BACKSPACE:
        //         return -1;
        //         break;
        //     }
        // }

        if (move) {
            auto state = get_state();
            auto p1_ate = p1->update(state);
            auto p2_ate = p2->update(state);

            if (p1_ate || p2_ate) {
                food = get_random_food();
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

    // void draw() {
    //     wmove(stdscr, 0, 0);
    //
    //     printw("P1: %d\n", p1->get_score());
    //     printw("P2: %d\n", p2->get_score());
    //
    //     for (auto j = 0; j < height; j++) {
    //         for (auto i = 0; i < width; i++) {
    //             if (p1->is_head(i, j)) {
    //                 printw("AA");
    //             } else if (p2->is_head(i, j)) {
    //                 printw("BB");
    //             } else if (p1->snake_contains(i, j)) {
    //                 printw("aa");
    //             } else if (p2->snake_contains(i, j)) {
    //                 printw("bb");
    //             } else if (i == foodX && j == foodY) {
    //                 printw("XX");
    //             } else {
    //                 printw("..");
    //             }
    //         }
    //         printw("\n");
    //     }
    //
    //     wrefresh(stdscr);
    // }

    void draw(Screen &screen) {
        screen.fill(color(0, 0, 0));
    }

    int run(Table &table) {
        // auto last_move = std::chrono::system_clock::now();
        time_t last_move = millis();
        while (true) {
            time_t now = millis();
            bool move = false;
            if (now - last_move > 300) {
                last_move = now;
                move = true;
            }

            auto update_res = update(move);
            if (update_res != 0) {
                return update_res + 1;
            }

            draw(table.screen);
        }
    }

  private:
    Point dims;
    Snake *p1;
    Snake *p2;
    Point food;
};

// digital pins
#define SCREEN_PIN 2
#define SW1_PIN 3
#define SW2_PIN 4

// analog pins
#define X1_PIN A0
#define Y1_PIN A1
#define X2_PIN A1
#define Y2_PIN A2

Table table(SCREEN_PIN, X1_PIN, Y1_PIN, SW1_PIN, X2_PIN, Y2_PIN, SW2_PIN);

void setup() { Serial.begin(9600); }

void loop() { Serial.println("Hello, world!"); }
