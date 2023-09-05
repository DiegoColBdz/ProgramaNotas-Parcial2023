#include "mbed.h"

const int numRows = 4;
const int numCols = 4;

char keyMap[numRows][numCols] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

DigitalOut rowPins[numRows] = {DigitalOut(D2), DigitalOut(D3), DigitalOut(D4), DigitalOut(D5)};
DigitalIn colPins[numCols] = {DigitalIn(D6), DigitalIn(D7), DigitalIn(D8), DigitalIn(D9)};

char inputBuffer[50];
int bufferIndex = 0;
bool recording = false;

char convertToGrade(float N) {
    char nota;
    
    if (N >= 0 && N <= 3) {
        nota = 'A';
    } else if (N > 3 && N <= 4) {
        nota = 'B';
    } else if (N > 4 && N <= 5) {
        nota = 'C';
    } else if (N > 5 && N <= 7) {
        nota = 'D';
    } else if (N > 7 && N < 9) {
        nota = 'E';
    } else if (N >= 9 && N <= 10) {
        nota = 'F';
    } else {
        nota = '?';  // Valor fuera de rango
    }
    
    return nota;
}

void processKey(char key) {
    if (key == '*' && !recording) {
        recording = true; // Iniciar la grabación del número
    } else if (key == '*' && recording) {
        if (bufferIndex > 0) {
            inputBuffer[bufferIndex] = '\0';  // Null-terminate the string
            printf("Numero ingresado: %s\n", inputBuffer);
            
            // Convertir la entrada a un número
            float N = atof(inputBuffer);
            
            if (N >= 0 && N <= 10) {
                char nota = convertToGrade(N);
                printf("Nota alfanumerica: %c\n", nota);
            } else {
                printf("Valor de nota no valido: %s\n", inputBuffer);
            }
            
            bufferIndex = 0;
        }
        recording = false; // Detener la grabación después de la conversión
    } else if (recording) {
        inputBuffer[bufferIndex++] = key;
    }
}

int main() {
    while (true) {
        for (int i = 0; i < numRows; i++) {
            rowPins[i] = 0;

            for (int j = 0; j < numCols; j++) {
                if (!colPins[j]) {
                    processKey(keyMap[i][j]);
                    ThisThread::sleep_for(500ms);  // Evita lecturas múltiples mientras la tecla está presionada
                }
            }

            rowPins[i] = 1;
        }
    }
}
