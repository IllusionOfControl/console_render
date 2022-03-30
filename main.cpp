#define _USE_MATH_DEFINES

#include <exception>
#include <cstdio>
#include <cstring>
#include <math.h>


const int SCREEN_WIDTH = 78;
const int SCREEN_HEIGHT = 40;

const float THETA_SPACING= 1.7f;
const float PHI_SPACING = 0.02f;
const float ANGLE_A_SPACING = 0.002f;
const float ANGLE_B_SPACING = 0.002f;
const float PI = M_PI;

const float THOR_RADIUS = 3.0f;
const float CYCLE_RADIUS = 1.0f;

const float Z_RANGE = 5.f;

const float RENDER_SCALE = 0.2f;
const float VIEW_DISTANCE = SCREEN_WIDTH * Z_RANGE / (CYCLE_RADIUS + THOR_RADIUS) * RENDER_SCALE;


template<class T>
struct Vertex3d {
    T x, y, z;
};


class GeometricObject {
protected:
    Vertex3d<float>* vertexArr_;
    int vertexCount_;
    virtual void Calculate() = 0;
public:
    virtual Vertex3d<float>* GetVertex() const = 0;
    virtual int GetVertexCount() const = 0;
};


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

    // TODO: use memset
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


class BasicRender {
protected:
    Buffer<int> *screenBuffer_;
    Buffer<float> *zBuffer_;
    GeometricObject* object_;
public:
    BasicRender(GeometricObject* obj, int render_width, int render_height) : object_(obj) {
        screenBuffer_ = new Buffer<int>(render_width, render_height);
        zBuffer_ = new Buffer<float>(render_width, render_height);
    }
    virtual Buffer<int>* Render() = 0;
    virtual void ClearBuffers() = 0;
};


class Circle : public GeometricObject {
private:
    const float radius_;
    const int vertexCount_;

    void Calculate() {
        float thetaSpacing = (2 * PI) / vertexCount_;
        Vertex3d<float> *vertex_ptr = vertexArr_;

        for (float theta = 0; theta < 2 * PI; theta += thetaSpacing) {
            float cos_theta = cosf(theta), sin_theta = sinf(theta);

            vertex_ptr->x = radius_ * cos_theta;
            vertex_ptr->y = radius_ * sin_theta;
            vertex_ptr->z = 0;

            vertex_ptr++;
        }
    }

public:
    Circle(float radius, int sectorCount) : radius_(radius), vertexCount_(sectorCount){
        vertexArr_ = (Vertex3d<float>*) malloc(sizeof(Vertex3d<float>) * vertexCount_);
        Calculate();
    }

    void Rotate() {}

    int GetVertexCount() const {
        return vertexCount_;
    }

    Vertex3d<float> * GetVertex() const {
        return vertexArr_;
    }
};


class CircleRender : public BasicRender {
private:

public:
    CircleRender(GeometricObject * object, int render_width, int render_height): 
            BasicRender(object, render_width, render_height) {}


    void ClearBuffers() {
        zBuffer_->Fill(0.f);
        screenBuffer_->Fill(0);
    }


    Buffer<int>* Render() {
        ClearBuffers();

        Vertex3d<float> *vertex_ptr = object_->GetVertex();
        
        for (int i = 0; i < object_->GetVertexCount(); i++) {
            float ooz = 1 / (vertex_ptr->z + Z_RANGE);

            int xp = (int) (SCREEN_WIDTH / 2.0f + VIEW_DISTANCE * ooz * vertex_ptr->x);
            int yp = (int) (SCREEN_HEIGHT / 2.0f - VIEW_DISTANCE * ooz * vertex_ptr->y);

            screenBuffer_->Update(xp, yp, 12);

            vertex_ptr++;
        }

        return screenBuffer_;
    }
};


class ConsoleEngine {
private:
    const int render_width_ = 78;
    const int render_height_ = 40;
    BasicRender *render_;
public:
    ConsoleEngine() : render_(nullptr) {}

    void SetRenderObject(GeometricObject *obj) {
        render_ = new CircleRender(obj, render_width_, render_height_);
    }

    void Loop() {
        Buffer<int> *buff = render_->Render();
        Draw(buff);
    }

    void Draw(Buffer<int> *buff) {
        printf("\x1b[H");
        char* line_buffer = new char[80];
        for (int y = 0; y < this->render_height_; y++) {
            for (int x = 0; x < this->render_width_; x++) {
                int int_index = buff->Get(x, y);
                line_buffer[x] = " .,-~:;=!*#$@"[int_index];
            }
            line_buffer[78] = '\n';
            line_buffer[79] = '\0';
            fprintf(stderr, line_buffer);
        }
    }
};


int main() {
    ConsoleEngine engine;
    Circle *circle = new Circle(3.0f, 90);
    engine.SetRenderObject(circle);
    
    engine.Loop();

    return 0;
}