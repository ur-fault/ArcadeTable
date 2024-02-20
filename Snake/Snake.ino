#include <Adafruit_NeoPixel.h>
#include <microTuple.h>

#define SCREEN_WIDTH (10)
#define SCREEN_HEIGHT (10)
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

#define JOYSTICK_THRESHOLD 0.5

typedef uint8_t port_t;
typedef uint16_t ledidx_t;
typedef uint32_t color_t;

typedef MicroTuple<int, int, int> Color;
// typedef MicroTuple<int, int> Point;
struct Point {
    int x, y;

    Point() : Point(0, 0) {}

    Point(int x, int y) : x(x), y(y) {}

    Point operator+(Point b) { return Point(x + b.x, y + b.y); }

    Point operator-(Point b) { return Point(x - b.x, y - b.y); }

    bool operator==(Point b) { return x == b.x && y == b.y; }
};

color_t color(int r, int g, int b) { return Adafruit_NeoPixel::Color(g, r, b); }

color_t color(Color color) {
    return Adafruit_NeoPixel::Color(color.first, color.rest.first,
                                    color.rest.rest.first);
}

/// A screen is a 2D array of pixels of size 10x10. Each pixel is a 3-tuple of
/// RGB values.
/// Port should be `2`
class Screen {
  public:
    Screen(port_t pin) : pixels(SCREEN_SIZE, pin, NEO_GRB + NEO_KHZ800) {
        this->pin = pin;
    }

    void setup() { pixels.begin(); }

    /// Sets the color of the pixel at (x, y) to the given color.
    void setPixel(int x, int y, color_t clr) {
        ledidx_t idx = getPixelIndex(x, y);
        if (idx != -1) {
            pixels.setPixelColor(idx, clr);
        }
    }

    /// Sets the color of the pixel at (x, y) to the given color.
    void setPixel(Point pos, color_t clr) { setPixel(pos.x, pos.y, clr); }

    void setPixel(int x, int y, Color clr) { setPixel(x, y, color(clr)); }

    void setPixel(Point pos, Color clr) { setPixel(pos.x, pos.y, clr); }

    void fill(color_t clr) { pixels.fill(clr); }

    void fill(Color clr) { fill(color(clr)); }

    void fill_rect(color_t clr, Point start, Point end) {
        for (int x = start.x; x <= end.x; x++) {
            for (int y = start.y; y <= end.y; y++) {
                setPixel(x, y, clr);
            }
        }
    }

    void show() { pixels.show(); }

    void clear() { pixels.clear(); }

    /// Gets index of pixel at (x, y) in the 1D array of pixels.
    /// Returns -1 if the pixel is out of bounds.
    /// LEDs trace out a snake pattern, so the first row is 0-9, the second is
    /// 19-10, the third is 20-29, etc.
    static ledidx_t getPixelIndex(int x, int y) {
        if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
            return -1;
        }

        if (y % 2 == 0) {
            return y * SCREEN_WIDTH + x;
        } else {
            return y * SCREEN_WIDTH + (SCREEN_WIDTH - x - 1);
        }
    }

    static ledidx_t getPixelIndex(Point pos) {
        return getPixelIndex(pos.x, pos.y);
    }

  private:
    port_t pin;
    Adafruit_NeoPixel pixels;
};

class Controls {
  public:
    Controls(port_t x, port_t y, port_t sw) {
        this->x = x;
        this->y = y;
        this->sw = sw;
    }

    void setup() { pinMode(sw, INPUT_PULLUP); }

    bool isPressed() { return digitalRead(sw) == HIGH; }

    /// Returns a tuple of states of directions of the joystick.
    /// The first element is the x direction, the second is the y direction.
    /// Values are either -1, 0, or 1.
    Point getJoystick() {
        int xVal = analogRead(x);
        int yVal = analogRead(y);

        Point pos(0, 0);

        if (xVal > 512 + 512 * JOYSTICK_THRESHOLD) {
            pos.x = 1;
        } else if (xVal < 512 - 512 * JOYSTICK_THRESHOLD) {
            pos.x = -1;
        }

        // // disable repeat
        // if (pos.x == joystickPrev.x) {
        //     pos.x = 0;
        // }

        if (yVal > 512 + 512 * JOYSTICK_THRESHOLD) {
            pos.y = 1;
        } else if (yVal < 512 - 512 * JOYSTICK_THRESHOLD) {
            pos.y = -1;
        }

        // // disable repeat
        // if (pos.y == joystickPrev.y) {
        //     pos.y = 0;
        // }

        if (pos == joystickPrev)
            pos = Point(0, 0);

        joystickPrev = pos;
        return pos;
    }

  private:
    port_t x, y, sw;
    Point joystickPrev;
};

class Table {
  public:
    Table(port_t screenPin, port_t x1, port_t y1, port_t sw1, port_t x2,
          port_t y2, port_t sw2)
        : screen(screenPin), controls1(x1, y1, sw1), controls2(x2, y2, sw2) {}

    void setup() {
        screen.setup();
        controls1.setup();
        controls2.setup();
    }

    Screen screen;
    Controls controls1;
    Controls controls2;
};
