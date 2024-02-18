#include <microTuple.h>

const int BOARD_SIZE = 10 * 10;

// typedef MicroTuple<int, int> Point;
typedef unsigned long time_t;

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
        direction = head.x == 0 ? Point(1, 0) : Point(-1, 0);
    }

    Snake(int x, int y) : Snake(Point(x, y)) {}

    bool update(const GameState &state) {
        body.insert(0, get_head() + direction);

        if (get_head() == state.food)
            return true;

        body.pop();
        return false;
    }

    bool is_dead(GameState &state, Snake &other) {
        auto head = get_head();

        auto x = head.x;
        auto y = head.y;

        auto width = state.dims.x;
        auto height = state.dims.y;

        if (x < 0 || x >= width || y < 0 || y >= height ||
            body_contains(x, y) || other.snake_contains(x, y)) {
            return true;
        }

        return false;
    }

    void set_direction(Point direction) { this->direction = direction; }

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
    Point direction;
};

class Game {
  public:
    time_t dl;

    Game(Point dims, time_t dl) {
        this->dims = dims;
        p1 = new Snake(0, 0);
        p2 = new Snake(dims - Point(1, 1));

        auto food = get_random_food();

        this->dl = dl;
    }

    ~Game() {
        delete p1;
        delete p2;
    }

    Point get_random_food() {
        return Point(random(dims.x), random(dims.y));
    }

    Point get_food() { return food; }

    GameState get_state() {
        return GameState{
            .dims = Point(dims),
            .food = get_food(),
        };
    }

    int update(bool move) {
        auto j1 = table.controls1.getJoystick();
        auto x1 = j1.x;
        auto y1 = j1.y;
        if (x1 < 0)
            this->p1->set_direction(Point(-1, 0));
        else if (x1 > 0)
            this->p1->set_direction(Point(1, 0));
        else if (y1 < 0)
            this->p1->set_direction(Point(0, -1));
        else if (y1 > 0)
            this->p1->set_direction(Point(0, 1));

        auto j2 = table.controls2.getJoystick();
        auto x2 = j1.x;
        auto y2 = j1.y;
        if (x2 < 0)
            this->p2->set_direction(Point(-1, 0));
        else if (x2 > 0)
            this->p2->set_direction(Point(1, 0));
        else if (y2 < 0)
            this->p2->set_direction(Point(0, -1));
        else if (y2 > 0)
            this->p2->set_direction(Point(0, 1));

        if (move) {
            auto state = get_state();
            auto p1_ate = p1->update(state);
            auto p2_ate = p2->update(state);

            if (p1_ate || p2_ate) {
                food = get_random_food();
            }

            auto p1_dead = p1->is_dead(state, *p2);
            auto p2_dead = p2->is_dead(state, *p1);

            if (p1_dead || p2_dead) {
                end = true;
                winner = p1_dead ? true : false;
            }

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

    void draw(Screen &screen) {
        screen.clear();

        screen.fill(color(0, 0, 0));

        for (auto y = 0; y < SCREEN_HEIGHT; y++) {
            for (auto x = 0; x < SCREEN_WIDTH; x++) {
                if (p1->is_head(x, y)) {
                    // screen.setPixel(color(255, 0, 0), Point(x * 10, y * 10), Point(10, 10));
                    screen.setPixel(x, y, color(255, 0, 0));
                } else if (p2->is_head(x, y)) {
                    // screen.fill_rect(color(0, 0, 255), Point(x * 10, y * 10), Point(10, 10));
                    screen.setPixel(x, y, color(0, 0, 255));
                } else if (p1->snake_contains(x, y)) {
                    // screen.fill_rect(color(255, 0, 0), Point(x * 10, y * 10), Point(10, 10));
                    screen.setPixel(x, y, color(128, 0, 0));
                } else if (p2->snake_contains(x, y)) {
                    // screen.fill_rect(color(0, 0, 255), Point(x * 10, y * 10), Point(10, 10));
                    screen.setPixel(x, y, color(0, 0, 128));
                } else if (x == food.x && y == food.y) {
                    // screen.fill_rect(color(0, 255, 0), Point(x * 10, y * 10), Point(10, 10));
                    screen.setPixel(x, y, color(0, 255, 0));
                }
            }
        }

        screen.show();
    }

    int run(Table &table) {
        // auto last_move = std::chrono::system_clock::now();
        time_t last_move = millis();
        while (true) {
            time_t now = millis();
            bool move = false;
            if (now - last_move > dl) {
                last_move = now;
                move = true;
            }

            auto update_res = update(move);
            if (update_res != 0)
                break;

            if (move)
                draw(table.screen);
        }

        table.screen.clear();
        table.screen.fill(color(0, 0, 0));

        color_t clr = winner ? color(255, 0, 0) : color(0, 0, 255);
        table.screen.fill_rect(clr, Point(0, 0), Point(SCREEN_WIDTH, SCREEN_HEIGHT));

        table.screen.show();

        while (table.controls1.isPressed() == false && table.controls2.isPressed() == false) {}
    }

  private:
    Point dims;
    Snake *p1;
    Snake *p2;
    Point food;
    bool end;
    bool winner;
};

void setup() {
    Serial.begin(9600);
    table.setup();
}

void loop() {
    Game game(Point(10, 10), 300);
    game.run(table);
}
