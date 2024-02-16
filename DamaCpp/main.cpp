#include <algorithm>
#include <cmath>
#include <iostream>
#include <optional>
#include <random>
#include <sstream>
#include <thread>
#include <tuple>
#include <variant>
#include <vector>

#include <curses.h>

template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }

struct Point {
    int x;
    int y;

    operator std::tuple<int, int>() const { return std::make_tuple(x, y); }

    Point operator+(const Point &other) const {
        return Point{x + other.x, y + other.y};
    }

    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }
};

struct Move {
    Point from, to;
    std::optional<Point> kill;

    std::ostream &operator<<(std::ostream &os) {
        os << "Move from " << from.x << " " << from.y << " to " << to.x << " "
           << to.y;
        if (kill) {
            os << " with kill at " << kill.value().x << " " << kill.value().y;
        }
        return os;
    }

    std::string to_string() {
        std::ostringstream os;
        os << this;
        return os.str();
    }
};

struct Cell {
    bool occupied;
    bool color;
    bool queen;

    Cell() : Cell(false, false) {}

    Cell(bool occupied, bool color, bool queen = false)
        : occupied(occupied), color(color), queen(queen) {}

    void occupy(bool color, bool queen = false) {
        occupied = true;
        this->color = color;
        this->queen = queen;
    }

    bool kill() {
        if (occupied) {
            occupied = false;
            return true;
        }
        return false;
    }

    bool move_here(Cell &cell) {
        bool prev_occ = occupied;

        // copy cell, from rust hehe
        *this = cell;

        return prev_occ;
    }

    void empty() { occupied = false; }

    bool upgrade() {
        if (queen || !occupied) {
            return false;
        }

        queen = true;
        return true;
    }

    std::string str() {
        if (occupied) {
            if (color) {
                if (queen) {
                    return "WQ";
                } else {
                    return "W ";
                }
            } else {
                if (queen) {
                    return "BQ";
                } else {
                    return "B ";
                }
            }
        } else {
            return "  ";
        }
    }

    short tcolor() {
        if (color) {
            return 1;
        } else {
            return 2;
        }
    }

    int dir() { return color ? 1 : -1; }
};

class Board {
  public:
    Board() {
        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 8; x++) {
                if ((y + x) % 2 == 0) {
                    get(x, y).occupy(true);
                }
            }
        }

        for (int y = 5; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                if ((y + x) % 2 == 0) {
                    get(x, y).occupy(false);
                }
            }
        }
    }

    Cell &operator[](Point p) { return cells[p.y][p.x]; }

    Cell &get(int x, int y) { return cells[y][x]; }

    Cell &get(Point p) { return cells[p.y][p.x]; }

    bool in_bounds(Point p) {
        return p.x >= 0 && p.x < 8 && p.y >= 0 && p.y < 8;
    }

    bool valid_pos(Point p) { return in_bounds(p) && (p.x + p.y) % 2 == 0; }

    std::variant<std::string, Move> get_move() {
        auto from = selected_from;
        auto to = selected_to;

        if (!valid_pos(from) || !valid_pos(to))
            return "Invalid position";

        auto [x, y] = from;
        auto [to_x, to_y] = to;

        auto cell = get(x, y);
        auto to_cell = get(to_x, to_y);

        auto move = Move{from, to};

        if (!cell.occupied)
            return "Unoccupied piece";
        if (to_cell.occupied)
            return "Occupied target";

        if (!cell.queen) {
            auto dx = to_x - x;
            auto dxa = std::abs(dx);
            auto dy = to_y - y;

            if (dxa != 1 && dxa != 2)
                return "Knight too far";

            if (sgn(dy) != cell.dir())
                return "Wrong direction";

            if (dxa == 2) {
                auto kill = Point{x + dx / 2, y + dy / 2};
                auto killed = get(kill);
                if (!killed.occupied || killed.color == cell.color)
                    return "Invalid kill";

                move.kill = kill;
            }
        } else {
        }

        return move;
    }

    void apply_move(Move &move) {
        auto [from, to, kill] = move;
        auto &from_cell = get(from);
        auto &to_cell = get(to);

        to_cell.move_here(from_cell);
        from_cell.empty();
        if (kill)
            get(kill.value()).kill();

        if (!to_cell.queen) {
            if (to_cell.color == false && to.y == 0)
                to_cell.upgrade();

            if (to_cell.color == true && to.y == 7)
                to_cell.upgrade();
        }

        last_move = move;
    }

    bool can_kill() {
        if (!last_move)
            return false;

        auto move = last_move.value();
        auto cell = get(move.to);

        if (!cell.queen) {
            auto dir = cell.dir();

            auto pos = move.to;

            if (in_bounds(pos + Point{-1, dir}) &&
                get(pos + Point{-1, dir}).occupied &&
                get(pos + Point{-1, dir}).color != cell.color &&
                !get(pos + Point{-2, dir * 2}).occupied) {
                return true;
            }

            if (in_bounds(pos + Point{-1, dir}) &&
                get(pos + Point{1, dir}).occupied &&
                get(pos + Point{1, dir}).color != cell.color &&
                !get(pos + Point{2, dir * 2}).occupied) {
                return true;
            }
        }

        return false;
    }

    int update() {
        int ch = getch();
        if (ch == ERR)
            return 0;

        auto sel = selecting_to ? selected_to : selected_from;

        switch (ch) {
        case 'w':
            sel.y--;
            break;
        case 's':
            sel.y++;
            break;
        case 'a':
            sel.x--;
            break;
        case 'd':
            sel.x++;
            break;
        }

        sel.x = std::clamp(sel.x, 0, 7);
        sel.y = std::clamp(sel.y, 0, 7);

        // first
        if (selecting_to)
            selected_to = sel;
        else
            selected_from = sel;

        // second
        *(selecting_to ? &selected_to : &selected_from) = sel;

        switch (ch) {
        case ' ':
            selecting_to = !selecting_to;
            break;
        }

        switch (ch) {
        case '\n':
            auto move_res = get_move();
            if (!std::holds_alternative<Move>(move_res))
                break;

            apply_move(std::get<Move>(move_res));

            if (!can_kill())
                color = !color;

            selecting_to = false;

            break;
        }

        switch (ch) {
        case KEY_BACKSPACE:
            return -1;
            break;
        }

        return 0;
    }

    void draw() {
        wmove(stdscr, 0, 0);

        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                auto p = Point{x, y};
                auto cell = get(x, y);

                attron(COLOR_PAIR(cell.tcolor()));
                if (selected_from == p || selected_to == p)
                    attron(A_STANDOUT);

                printw("%s", cell.str().c_str());

                if (selected_from == p || selected_to == p)
                    attroff(A_STANDOUT);
                attroff(COLOR_PAIR(cell.tcolor()));
            }
            printw("\n");
        }

        printw("\n");

        if (color)
            printw("Black's turn\n");
        else
            printw("White's turn\n");

        if (selecting_to)
            printw("Selecting target\n");
        else
            printw("Selecting piece\n");

        clrtoeol();
        auto move_res = get_move();
        if (std::holds_alternative<std::string>(move_res))
            printw("Invalid: %s\n", std::get<std::string>(move_res).c_str());
        else {
            printw("Valid move\n");
        }

        wrefresh(stdscr);
    }

    Cell cells[8][8];
    Point selected_from = Point{0, 0}, selected_to = Point{1, 1};
    bool selecting_to = false;
    bool color = false;

    std::optional<Move> last_move;
};

int main() {
    // init screan
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, true);
    keypad(stdscr, true);

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);

    // restore terminal on exit
    atexit([]() { endwin(); });

    // game code
    Board board;

    // board[{1, 3}].occupy(false, false);
    // board.selected_from = Point{0, 2};
    // board.selected_to = Point{2, 4};

    while (true) {
        if (board.update() != 0)
            break;

        board.draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
