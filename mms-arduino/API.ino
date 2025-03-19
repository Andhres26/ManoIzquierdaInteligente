// ----- API -----

void log(String message) {
    Serial.print("log " + message + "\n");
}

int mazeWidth() {
    return getInteger("mazeWidth");
}

int mazeHeight() {
    return getInteger("mazeHeight");
}

bool wallFront() {
    return getBoolean("wallFront");
}

bool wallRight() {
    return getBoolean("wallRight");
}

bool wallLeft() {
    return getBoolean("wallLeft");
}

bool moveForward() {
    //pasos++;
    //move(1,100);
    return getAck("moveForward");
}


void turnRight() {  
    
    getAck("turnRight");
}

void turnLeft() { 
    
    getAck("turnLeft");
}

void setWall(int x, int y, char direction) {
    Serial.print(
        "setWall "
        + String(x) + " "
        + String(y) + " "
        + String(direction) + "\n"
    );
}

void clearWall(int x, int y, char direction) {
    Serial.print(
        "clearWall "
        + String(x) + " "
        + String(y) + " "
        + String(direction) + "\n"
    );
}

void setColor(int x, int y, char color) {
    Serial.print(
        "setColor "
        + String(x) + " "
        + String(y) + " "
        + String(color) + "\n"
    );
}

void clearColor(int x, int y) {
    Serial.print(
        "clearColor "
        + String(x) + " "
        + String(y) + "\n"
    );
}

void clearAllColor() {
    Serial.print("clearAllColor\n");
}

void setText(int x, int y, String text) {
    Serial.print(
        "setText "
        + String(x) + " "
        + String(y) + " "
        + text + "\n"
    );
}

void clearText(int x, int y) {
    Serial.print(
        "clearText "
        + String(x) + " "
        + String(y) + "\n"
    );
}

void clearAllText() {
    Serial.print("clearAllText\n");
}

bool wasReset() {
    return getBoolean("wasReset");
}

void ackReset() {
    getAck("ackReset");
}

// ----- Helpers -----

String readline() {
    String response = "";
    while (response == "") {
        response = Serial.readStringUntil('\n');
    }
    return response;
}

String communicate(String command) {
    Serial.print(command + "\n");
    return readline();
}

bool getAck(String command) {
    String response = communicate(command);
    return response == "ack";
}

bool getBoolean(String command) {
    String response = communicate(command);
    return response == "true";
}

int getInteger(String command) {
    String response = communicate(command);
    return response.toInt();
}


/// /////////////////////FUNCIONES /////////////////////////////////
// Función para actualizar la posición basada en la dirección actual
void actualizarPosicion() {
    if (direccionActual == 0 && posY < SIZE - 1) posY++;  // Arriba
    else if (direccionActual == 1 && posX < SIZE - 1) posX++;  // Derecha
    else if (direccionActual == 2 && posY > 0) posY--;  // Abajo
    else if (direccionActual == 3 && posX > 0) posX--;  // Izquierda
}


// Funciones para girar y actualizar dirección
void girarIzquierda() {
    move(3,100);
    direccionActual = (direccionActual + 3) % 4;
    turnLeft();
}

void girarDerecha() {
    move(4,100);
    direccionActual = (direccionActual + 1) % 4;
    turnRight();
}

void girar180() {
    direccionActual = (direccionActual + 2) % 4;
    turnRight();
    move(4,100);
    turnRight();
    move(4,100);
}



// Función para verificar si hay una celda sin visitas en una dirección dada
bool hayCeldaNueva(int direccion) {
    int newX = posX, newY = posY;

    if (direccion == 0) newY++;  // Arriba
    else if (direccion == 1) newX++;  // Derecha
    else if (direccion == 2) newY--;  // Abajo
    else if (direccion == 3) newX--;  // Izquierda

    return visitados[newX][newY] == 0;
}

// Función para verificar si hay una celda sin visitas a la derecha y sin pared
bool hayCeldaNuevaDerecha() {
    return !wallRight() && hayCeldaNueva((direccionActual + 1) % 4);
}

// Función para verificar si hay una celda sin visitas al frente y sin pared
bool hayCeldaNuevaFrente() {
    return !wallFront() && hayCeldaNueva(direccionActual);
}




//////////////////////////////FUNCIONES MBOT /////////////////////////////

void move(int direction, int speed) {
  int leftSpeed = 0;
  int rightSpeed = 0;
  if(direction == 1) { //avanzar
    leftSpeed = speed;
    rightSpeed = speed;
  } else if(direction == 2) { //retroceder
    leftSpeed = -speed;
    rightSpeed = -speed;
  } else if(direction == 3) { // izquierda
    leftSpeed = -speed;
    rightSpeed = speed;
  } else if(direction == 4) { // derecha
    leftSpeed = speed;
    rightSpeed = -speed;
  }
  motor_9.run((9) == M1 ? -(leftSpeed) : (leftSpeed));
  motor_10.run((10) == M1 ? -(rightSpeed) : (rightSpeed));
}