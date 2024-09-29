#include <windows.h>
#include <iostream>
#include <cmath>
#include <conio.h>
#include <stdlib.h>

float f0(float x) {
    return sin(x);
}
float f1(float x) {
    return cos(x);
}
float f2(float x) {
    return x * x;
}
float f3(float x) {
    return x * x * x;
}
float f4(float x) {
    if (x == 0) return 0;
    return (1 / x) * 10;
}
float f5(float x) {
    return cos(x * x + 6);
}
float f6(float x) {
    return 2 * x + 6;
}
float f7(float x) {
    if (x < 0)return x * x;
    if (0 <= x && x < 4) return x;
    return 4;
}
float f8(float x) {
    if (x > 0)return  -x + 2;
    return x * x + 2 * x + 1;
}
float f9(float x) {
    if (x < 0)return log2(sin(x));
    return log10(x * x);
}

float lagrange_interpolation(float x, float* xi, float* yi, int n) {
    float result = 0.0;

    for (int i = 0; i < n; i++) {
        float term = yi[i];
        for (int j = 0; j < n; j++) {
            if (j != i) {
                term = term * (x - xi[j]) / (xi[i] - xi[j]);
            }
        }
        result += term;
    }

    return result;
}

float newton_interpolation(float x, float* xi, float* yi, int n) {
float result = 0.0;

float** divided_diff = new float* [n];
for (int i = 0; i < n; i++) {
    divided_diff[i] = new float[n];
    divided_diff[i][0] = yi[i]; 
}

for (int j = 1; j < n; j++) {
    for (int i = 0; i < n - j; i++) {
        divided_diff[i][j] = (divided_diff[i + 1][j - 1] - divided_diff[i][j - 1]) / (xi[i + j] - xi[i]);
    }
}

for (int j = 0; j < n; j++) {
    float term = divided_diff[0][j];
    for (int k = 0; k < j; k++) {
        term *= (x - xi[k]);
    }
    result += term;
}

for (int i = 0; i < n; i++) {
    delete[] divided_diff[i];
}
delete[] divided_diff;

return result;
}

int main() {
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_SHOWMAXIMIZED);
    HDC hdc = GetDC(hwnd);

    while (true) {
        float a = -30, b = 30;
        float h = 0.01;
        system("cls");

        std::cout << "(" << a << "," << b << ")" << h << std::endl;
        std::cout << "0: sin x" << std::endl << "1: cos x" << std::endl << "2: x^2" << std::endl << "3: x^3" << std::endl << "4: (1/x)*10" << std::endl;
        std::cout << "5: cos (x^2 + 6)" << std::endl << "6: 2x+6" << std::endl << "7: x^2 then x<0\n   x then 0<=x<4\n   4 then x>=4" << std::endl <<
            "8: -x+2 then x>0\n   x^2+2x+1 then x<=0" << std::endl << "9: log2(sin x) then x<0\n   log10(x^2) then x>=0" << std::endl;
        
        for (int x = 0; x < 1920; x++) SetPixel(hdc, x, 1080 / 2, RGB(255, 255, 255));
        for (int x = 0; x < 1080; x++) SetPixel(hdc, 1920 / 2, x, RGB(255, 255, 255));
        for (int x = 0; x < 1920; x += 10) for (int y = 0; y < 9; y++) SetPixel(hdc, x, y + 536, RGB(255, 255, 255));
        for (int x = 0; x < 1080; x += 10) for (int y = 0; y < 9; y++) SetPixel(hdc, y + 956, x, RGB(255, 255, 255));
        
        char selectedFunction;
        selectedFunction = _getch();
        
        float (*selectedFunc)(float);
        switch (selectedFunction) {
        case '0':
            selectedFunc = f0;
            break;
        case '1':
            selectedFunc = f1;
            break;
        case '2':
            selectedFunc = f2;
            break;
        case '3':
            selectedFunc = f3;
            break;
        case '4':
            selectedFunc = f4;
            break;
        case '5':
            selectedFunc = f5;
            break;
        case '6':
            selectedFunc = f6;
            break;
        case '7':
            selectedFunc = f7;
            break;
        case '8':
            selectedFunc = f8;
            break;
        case '9':
            selectedFunc = f9;
            break;
        default:
            selectedFunc = f0;
            break;
        }

        for (float x = a; x < b; x += h)
            SetPixel(hdc, (x + 96) * 10, (-1 * selectedFunc(x) + 54) * 10, RGB(42, 213, 200));
        char dFunction;
        dFunction = _getch();
        if (dFunction == '1') {
            float getX[15] = { 0 };
            float getY[15] = { 0 };
            for (float x = a; x < b; x += h) {
                int index = static_cast<int>((x + 30) / 4);
                getX[index] = x;
                getY[index] = selectedFunc(x);
            }

            for (float x = a; x < b; x += h) {
                float interpolatedY = newton_interpolation(x, getX, getY, 15);
                SetPixel(hdc, (x + 96) * 10, (-1 * interpolatedY + 54) * 10, RGB(255, 0, 0));
            }
            std::cout << "newton";
        } else {
            float getX[30] = { 0 };
            float getY[30] = { 0 };
            for (float x = a; x < b; x += h) {
                int index = static_cast<int>((x + 30) / 2);
                getX[index] = x;
                getY[index] = selectedFunc(x);
            }

            for (float x = a; x < b; x += h) {
                float interpolatedY = lagrange_interpolation(x, getX, getY, 30);
                SetPixel(hdc, (x + 96) * 10, (-1 * interpolatedY + 54) * 10, RGB(255, 0, 0));
            }
            std::cout << "lagrange";
        }
        
        if (selectedFunction == 'e') break;
        selectedFunction = _getch();
        if (selectedFunction == 'e') break;
        system("cls");
        Sleep(100);
    }

    ReleaseDC(hwnd, hdc);
    return 0;
}