#include <stdio.h>
#include <math.h>
#include <string.h>
#include "constants.h"


struct RotationAngles {
    float x, z;
} ;
typedef struct RotationAngles RotationAngles;


void render(RotationAngles *rot_angles, char *screenBuffer, char *lightingBuffer) {
    float rotSinX = sinf(rot_angles->x);
    float rotCosX = cosf(rot_angles->x);
    float rotSinZ = sinf(rot_angles->z);
    float rotCosZ = cosf(rot_angles->z);

    float thetaSpacing = (PI * 2) / THOR_SEGMENTS;
    for (float theta = 0; theta < (PI * 2); theta += thetaSpacing) {
        float thetaSin = sinf(theta);
        float thetaCos = cosf(theta);

        float phiSpacing = (PI * 2) / CIRCLE_SEGMENTS;
        for (float phi = 0; phi < (PI * 2); phi += phiSpacing) {
            float phiSin = sinf(theta);
            float phiCos = cosf(theta);

            float circleX = CIRCLE_RADIUS + THOR_RADIUS * thetaCos;
            float circleY = THOR_RADIUS * thetaSin;
            
            float x = circleX;
            float y = circleY;
            float z = Z_RANGE;

            float ooz = 1 / z;

            int xp = (int)(SCREEN_WIDTH / 2 + VIEW_DISTANCE * ooz * x);
            int yp = (int)(SCREEN_HEIGHT / 2 - VIEW_DISTANCE * ooz * y);

            screenBuffer[yp * SCREEN_WIDTH + xp] = '@';
        }
    }
}

void draw(char *screenbuffer) {
    printf("\x1b[H");
    char* line_buffer = malloc(sizeof(char) * 80);
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            line_buffer[x] = screenbuffer[y * SCREEN_WIDTH + x];
        }
        line_buffer[78] = '\n';
        line_buffer[79] = '\0';
        fprintf(stderr, line_buffer);
    }
}


int main() {
    const size_t bufferSize = SCREEN_WIDTH * SCREEN_HEIGHT;
    
    RotationAngles angles = { 1.0f, 0.0f };
    char* screenBuffer = malloc(sizeof(char) * bufferSize);
    float* lightBuffer = malloc(sizeof(char) * bufferSize);

    for (;;) {
        memset(screenBuffer, ' ', bufferSize);
        memset(lightBuffer, 0.0f, bufferSize);

        render(&angles, screenBuffer, lightBuffer);
        draw(screenBuffer);
    }
    return 0;
}