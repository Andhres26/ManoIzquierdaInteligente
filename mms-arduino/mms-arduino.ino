#include <MeMCore.h>
#include <Wire.h>
#define SIZE 16 // Tamaño del laberinto

int posX = 0, posY = 0;  // Posición inicial del robot
uint8_t visitados[SIZE][SIZE]; // Reducir a uint8_t para ahorrar memoria
bool escapando = false;  
int direccionActual = 0;  // 0 = arriba, 1 = derecha, 2 = abajo, 3 = izquierda
int contadorMeta = 0;
int pasos = 0; // Contador de pasos

MeDCMotor motor_9(9);
MeDCMotor motor_10(10);

void setup() {
    Serial.begin(19200);    
    log("Bob conectado");
    setColor(0, 0, 'G');
    setText(0, 0, "Inicio");

    memset(visitados, 0, sizeof(visitados)); // Inicializar matriz de visitas
}

void loop() {      
    // VERIFICAR SI SE LLEGÓ A LA META
     // Verificar si se llegó al centro del laberinto
    if (posX == SIZE / 2 && posY == SIZE / 2) {
        log("¡¡¡¡¡¡Meta detectada!!!!!!");
        log("Pasos: " + String(pasos));
        delay(5000);
        return;
    }

    char buffer[30];
    sprintf(buffer, "Pos: %d,%d", posX, posY);
    log(buffer);

    visitados[posX][posY]++;
    pasos++;  // Contar cada movimiento

    if (visitados[posX][posY] > 1 && !escapando) { 
        escapando = true;
        log("Escape activado...");
    }

    // MODO ESCAPE
    if (escapando) {
        if (hayCeldaNuevaDerecha()) {
            log("Esc: Derecha");
            girarDerecha();
            moveForward();
            actualizarPosicion();
            escapando = false;
            return;
        }
        if (hayCeldaNuevaFrente() && visitados[posX][posY] > 4) {
            log("Esc: Frente");
            moveForward();
            actualizarPosicion();
            escapando = false;
            return;
        }
        if (!wallRight() && visitados[posX][posY] > 6){
            log("Esc: Ultima derecha");
            girarDerecha();
            moveForward();
            actualizarPosicion();
            escapando = false;
            return;
        }
        if (!wallFront() && visitados[posX][posY] > 6){
            log("Esc: Ultima frente");
            moveForward();
            actualizarPosicion();
            escapando = false;
            return;
        }
    }

    // ESTRATEGIA DE LA MANO IZQUIERDA
    if (!wallLeft()) {
        log("Izquierda");
        girarIzquierda();
        moveForward();
        actualizarPosicion();
    } else if (!wallFront()) {
        log("Avanza");
        moveForward();
        actualizarPosicion();
    } else if (!wallRight()) {
        log("Derecha");
        girarDerecha();
        moveForward();
        actualizarPosicion();
    } else {
        log("Giro 180");
        girar180();
    }
}

