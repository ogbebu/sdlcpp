#include <cmath>

struct vec2 {
    float x = 0.0f;
    float y = 0.0f;

    vec2() = default;
    vec2(float x, float y) : x(x), y(y) {}

    // --- arithmetic (vec2 op vec2) ---
    vec2 operator+(const vec2& other) const { return { x + other.x, y + other.y }; }
    vec2 operator-(const vec2& other) const { return { x - other.x, y - other.y }; }
    vec2 operator*(const vec2& other) const { return { x * other.x, y * other.y }; } // component-wise
    vec2 operator/(const vec2& other) const { return { x / other.x, y / other.y }; } // component-wise

    // --- arithmetic (vec2 op scalar) ---
    vec2 operator*(float scalar) const { return { x * scalar, y * scalar }; }
    vec2 operator/(float scalar) const { return { x / scalar, y / scalar }; }

    // --- unary ---
    vec2 operator-() const { return { -x, -y }; }

    // --- compound assignment ---
    vec2& operator+=(const vec2& other) { x += other.x; y += other.y; return *this; }
    vec2& operator-=(const vec2& other) { x -= other.x; y -= other.y; return *this; }
    vec2& operator*=(const vec2& other) { x *= other.x; y *= other.y; return *this; } //
    vec2& operator/=(const vec2& other) { x /= other.x; y /= other.y; return *this; }
    vec2& operator*=(float scalar) { x = x * scalar; y = y * scalar; return *this; } //
    vec2& operator/=(float scalar) { x = x / scalar; y = y / scalar; return *this; }

    // --- comparison ---
    bool operator==(const vec2& other) const { return x == other.x && y == other.y; }
    bool operator!=(const vec2& other) const { return !(*this == other); }

    // --- utility ---
    float length() const { return std::sqrt(x * x + y * y); }
    float lengthSq() const { return x * x + y * y; }

    float dot(const vec2& other) const { return x * other.x + y * other.y; }

    vec2 normalized() const {
        float len = length();
        return len > 0.0f ? vec2{ x / len, y / len } : vec2{ 0.0f, 0.0f };
    }

    void normalize() {
        float len = length();
        if (len > 0.0f) { x /= len; y /= len; }
    }
};

// ignore vec2. it just stores float x and float y. 

// this struct stores information about the player yeah soo its understood
struct Entity {
    vec2 size{100,100};
    vec2 pos{100,100};
    float speed = 100.0f;
    vec2 direction{0,0}; //x,y
    SDL_Texture* texture = nullptr;
};

// we doing pos = direction * speed * deltaTime each frame
// and diretion is a unit vector that we edit on WASD. ohh yeahh a bit similar to Unity yeah
// less add another player and you control the first with WAS and i controll the second with arrow keys!