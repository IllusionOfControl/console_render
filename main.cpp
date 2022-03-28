#define _USE_MATH_DEFINES

#include <exception>
#include <cstdio>
#include <math.h>


const int SCREEN_WIDTH = 78;
const int SCREEN_HEIGHT = 40;

const float THETA_SPACING= 0.02f;
const float PI = M_PI;

const float THOR_RADIUS = 1.0f;

const float Z_RANGE = 20.f;

const float RENDER_SCALE = 0.1f;
const float VIEW_DISTANCE = SCREEN_WIDTH * Z_RANGE / THOR_RADIUS * RENDER_SCALE;


template<class T>
class Buffer
{
private:
    T* _buffer;
    const int _width, _height;

public:
    Buffer(int width, int height) : _width(width), _height(height) {
        size_t buffer_size = width * height;
        _buffer = new T[buffer_size];
    }

    bool Update(int x, int y, T value) {
        if (x < this->_width && x >= 0 &&
            y < this->_height && y >= 0) {
            int location = y * _width + x;
            _buffer[location] = value;
            return true;
        }
        return false;
    }

    void Fill(const T value) {
        size_t buffer_size = _width * _height;
        for (size_t index = 0; index < buffer_size; index++) {
            _buffer[index] = value;
        }
    }

    T Get(int x, int y) const {
        if (x < this->_width && y < this->_height) {
            int location = y * _width + x;
            return _buffer[location];
            location++;
        }
        return 0;
    }
};


class ConsoleRender {
private:
    Buffer<float>* _z_buffer;
    Buffer<char>* _ascii_buffer;

    const int _render_width, _render_height;

public:
    ConsoleRender(const int render_width, const int render_height) 
            : _render_width(render_width), _render_height(render_height) {
        _z_buffer = new Buffer<float>(render_width, render_height);
        _ascii_buffer = new Buffer<char>(render_width, render_height);
    }

    void Render() {
        _z_buffer->Fill(0.f);
        _ascii_buffer->Fill(' ');

        for (float theta = 0; theta < 2 * PI; theta += THETA_SPACING) {
            float cos_theta = cosf(theta), sin_theta = sinf(theta);

            float circle_x = THOR_RADIUS * cos_theta;
            float circle_y = THOR_RADIUS * sin_theta;

            float x = circle_x;
            float y = circle_y;

            float z = Z_RANGE;
            float ooz = 1 / z;

            int xp = (int)(_render_width / 2.0f + VIEW_DISTANCE * ooz * x);
            int yp = (int)(_render_height / 2.0f - VIEW_DISTANCE * ooz * y);

            _ascii_buffer->Update(xp, yp, '@');
        }
    }

    void Draw() {
        printf("\x1b[H");
        char* line_buffer = new char[80];
        for (int y = 0; y < this->_render_height; y++) {
            for (int x = 0; x < this->_render_width; x++) {
                line_buffer[x] = _ascii_buffer->Get(x, y);
            }
            line_buffer[78] = '\n';
            line_buffer[79] = '\0';
            fprintf(stderr, line_buffer);
        }
    }
};


int main() {
    ConsoleRender render(SCREEN_WIDTH, SCREEN_HEIGHT);
    render.Render();
    render.Draw();
    return 0;
}