#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "constants.h"


struct RotationAngles {
    float x, z;
} ;
typedef struct RotationAngles RotationAngles;


void render(const RotationAngles *rot_angles, char *screenBuffer, float *zBuffer) {
    const float viewDistance = SCREEN_WIDTH * Z_RANGE / THOR_RADIUS * RENDER_SCALE;

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
            float phiSin = sinf(phi);
            float phiCos = cosf(phi);

            float circleX = THOR_RADIUS + CIRCLE_RADIUS * thetaCos;
            float circleY = CIRCLE_RADIUS * thetaSin;

            float x = circleX * (rotCosZ * phiCos + rotSinX * rotSinZ * phiSin)
                - circleY * rotCosX * rotSinZ;
            float y = circleX * (rotSinZ * phiCos - rotSinX * rotCosZ * phiSin)
                + circleY * rotCosX * rotCosZ;
            float z = Z_RANGE + rotCosX * circleX * phiSin + circleY * rotSinX;

            float ooz = 1 / z;

            int xp = (int)(SCREEN_WIDTH / 2 + viewDistance * ooz * x);
            int yp = (int)(SCREEN_HEIGHT / 2 - viewDistance * ooz * y);

            if (xp < 0.f || xp > SCREEN_WIDTH || yp < 0.f || yp > SCREEN_HEIGHT) continue;

            float light = phiCos * thetaCos * rotSinZ - rotCosX * thetaCos * phiSin -
                rotSinX * thetaSin + rotCosZ * (rotCosX * thetaSin - thetaCos * rotSinX * phiSin);
            
            if (light > 0) {
                if (ooz > zBuffer[yp * SCREEN_WIDTH + xp]) {
                    zBuffer[yp * SCREEN_WIDTH + xp] = ooz;
                    int luminance_index = light * 8;
                    screenBuffer[yp * SCREEN_WIDTH + xp] = ".,-~:;=!*#$@"[luminance_index];
                }
            }
        }
    }
}


void draw(char *screenbuffer) {
    printf("\x1b[H");
    char *line_buffer = (char*) malloc(sizeof(char) * 80);

    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            line_buffer[x] = screenbuffer[y * SCREEN_WIDTH + x];
        }
        line_buffer[78] = '\n';
        line_buffer[79] = '\0';
        printf("%s", line_buffer);
    }
    free(line_buffer);
}


int main() {
    const size_t bufferSize = SCREEN_WIDTH * SCREEN_HEIGHT;
    
    RotationAngles angles = { 0.0f, 0.0f };
    char *screenBuffer = (char*) malloc(sizeof(char) * bufferSize);
    float *zBuffer = (float*) malloc(sizeof(float) * bufferSize);

    if (screenBuffer == NULL || zBuffer == NULL) {
        return 1;
    }

    for (;;) {
        for (size_t i = 0; i < bufferSize; ++i) {
            screenBuffer[i] = ' ';
        }
        
        for (size_t i = 0; i < bufferSize; ++i) {
            zBuffer[i] = 0.0f;
        }

        render(&angles, screenBuffer, zBuffer);
        draw(screenBuffer);

        angles.x += ROTATE_X_ANGLE;
        angles.z += ROTATE_Z_ANGLE;
    }

    free(screenBuffer);
    free(zBuffer);

    return 0;
}