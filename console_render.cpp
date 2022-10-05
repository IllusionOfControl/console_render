#include <iostream>
#include <Windows.h>
#include "math.h"
#include "objects.h"


//void render_screen(wchar_t* screen, int tick) {
//	
//}


const char GRADIENT[] = " .:!/r(l1Z4H9W8$@";



int main() {
	int width = 120 * 2;
	int height = 30 * 2;
	//SetWindow(width, height);
	float aspect = (float)width / height;
	float pixelAspect = 11.0f / 24.0f;
	int gradientSize = std::size(GRADIENT) - 2;

	wchar_t* screen = new wchar_t[width * height];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	for (int t = 0; t < 10000; t++) {
		vector3 light = norm(vector3(-0.5, 0.5, -1.0));
		//vector3 light = norm(vector3(sin(t*0.01), cos(t * 0.01), -1.0));
		vector3 spherePos = vector3(0, -3, 0);
		//vector3 spherePos = vector3(0, 0, 0);
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				vector2 uv = vector2(i, j) / vector2(width, height) * 2.0f - 1.0f;
				uv.x *= aspect * pixelAspect;
				vector3 ro = vector3(-6, 0, 0);
				vector3 rd = norm(vector3(2, uv));
				ro = rotate(ro, { 0.f, 0.25f, 0.01f * t });
				rd = rotate(rd, { 0.f, 0.25f, 0.01f * t });
				float diff = 0;
				for (int k = 0; k < 5; k++) {
					float minIt = 99999;
					vector2 intersection = sphere(ro - spherePos, rd, 1);
					vector3 n = 0;
					float albedo = 1;
					if (intersection.x > 0) {
						vector3 itPoint = ro - spherePos + rd * intersection.x;
						minIt = intersection.x;
						n = norm(itPoint);
					}
					vector3 boxN = 0;
					//intersection = box(ro, rd, 1, boxN);
					intersection = box(ro, rd, 1, boxN);
					if (intersection.x > 0 && intersection.x < minIt) {
						minIt = intersection.x;
						n = boxN;
					}
					vector3 box_2N = 0;
					intersection = box(ro - {0, -4, 0}, rd, 1, box_2N);
					if (intersection.x > 0 && intersection.x < minIt) {
						minIt = intersection.x;
						n = box_2N;
					}
					//intersection = plane(ro, rd, vector3(0, 0, -1), 1);
					//if (intersection.x > 0 && intersection.x < minIt) {
					//	minIt = intersection.x;
					//	n = vector3(0, 0, -1);
					//	albedo = 0.5;
					//}
					if (minIt < 99999) {
						diff = (dot(n, light) * 0.5 + 0.5) * albedo;
						ro = ro + rd * (minIt - 0.01);
						rd = reflect(rd, n);
					}
					else break;
				}
				int color = (int)(diff * 20);
				color = clamp(color, 0, gradientSize);
				char pixel = GRADIENT[color];
				screen[i + j * width] = pixel;
			}
		}
		screen[width * height - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, width * height, { 0, 0 }, &dwBytesWritten);
	}
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
