// Includes
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

// Declares Floats
float A, B, C;
float X, Y, Z;
float cubeWidth;
float zBuffer[160 * 44];
float horizontalOffSet;
float k1 = 40;
float ooz;
float incrementalSpeed = 0.6;

// Declares Integers
int width = 160;
int height = 44;
int distanceFromCam = 100;
int backgroundASCIICode = '.';
int xp, yp, zp;
int indx;

// Declares Characters
char buffer[160 * 44];

// Calculates X Value of Cube Surface
float calculateX(int x, int y, int z) {
    float calcX = (y * sin(A) * sin(B) * cos(C) - z * cos(A) * sin(B) * cos(C) + y * cos(A) * sin(C) + z * sin(A) * sin(C) + x * cos(B) * cos(C));
    return calcX;
}

// Calculates Y Value of Cube Surface
float calculateY(int x, int y, int z) {
    float calcY = (y * cos(A) * cos(C) + z * sin(A) * cos(C) - y * sin(A) * sin(B) * sin(C) + z * cos(A) * sin(B) * sin(C) - x * cos(B) * sin(C));
    return calcY;
}

// Calculates Z Value of Cube Surface
float calculateZ(int x, int y, int z) {
    float calcZ = (z * cos(A) * cos(B) - y * sin(A) * cos(B) + x * sin(B));
    return calcZ;
}

// Calculates Cube's Surface Area
void calculateSurfaceArea(float cubeX, float cubeY, float cubeZ, int ch) {
    X = calculateX(cubeX, cubeY, cubeZ);
    Y = calculateY(cubeX, cubeY, cubeZ);
    Z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;
    
    ooz = 1/Z;

    xp = (int)(width / 2 + horizontalOffSet + k1 * ooz * X * 2);
    yp = (int)(height / 2 + k1 * ooz * Y);

    indx = xp + yp * width;

    if(indx >= 0 && indx < width * height) {
        if(ooz > zBuffer[indx]) {
            zBuffer[indx] = ooz;
            buffer[indx] = ch;
        }
    }
}

// Generates a new Cube based on given width and horizontal display offset
void generateCube(float cWidth, float hOffSet) {
    horizontalOffSet = hOffSet;
    cubeWidth = cWidth;
    for(float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementalSpeed) {
        for(float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementalSpeed) {
            calculateSurfaceArea(cubeX, cubeY, -cubeWidth, '@');
            calculateSurfaceArea(cubeWidth, cubeY, cubeX, '$');
            calculateSurfaceArea(-cubeWidth, cubeY, -cubeX, '~');
            calculateSurfaceArea(-cubeX, cubeY, cubeWidth, '#');
            calculateSurfaceArea(cubeX, -cubeWidth, -cubeY, ';');
            calculateSurfaceArea(cubeX, cubeWidth, cubeY, '+');        
        }
    }
}

// Main Function
int main() {
    printf("\x1b[2J");
    for(;;) {
        memset(buffer, backgroundASCIICode, width * height);
        memset(zBuffer, 0, width * height * 4);
        // Cube #1
        generateCube(20, (-2 * cubeWidth));
        // Cube #2
        generateCube(15, (2 * cubeWidth));

        printf("\x1b[H");
        for(int i = 0; i < width * height; i++) {
            putchar(i % width ? buffer[i] : 10);
        }

        A += 0.05;
        B += 0.05;
        C += 0.01;
        usleep(8000 * 2);
    }
    return 0;
}
