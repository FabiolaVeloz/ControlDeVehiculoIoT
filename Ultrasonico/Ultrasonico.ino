// Pines del sensor ultrasónico
const int Trigger = 25;
const int Echo = 34;

// Pines para el control de los motores
int in1A = 32, in2A = 33;  // Control para motor izquierdo
int in1B = 13, in2B = 26;  // Control para motor derecho
int enaA = 27, enbA = 14;  // Pines PWM para velocidad de los motores

unsigned int tiempo; //Tiempo únicamente positivo
unsigned int distanciaActual;
int girosDerecha = 0;  // Contador de giros a la derecha

void setup() {
  Serial.begin(115200); // Comunicación serial
  pinMode(Trigger, OUTPUT); // Pin Trigger como salida
  pinMode(Echo, INPUT);     // Pin Echo como entrada
  digitalWrite(Trigger, LOW);
  
  // Pines de los motores
  pinMode(in1A, OUTPUT);
  pinMode(in2A, OUTPUT);
  pinMode(in1B, OUTPUT);
  pinMode(in2B, OUTPUT);
  pinMode(enaA, OUTPUT);
  pinMode(enbA, OUTPUT);
}

// Función general para mover el carrito
void mover(int dirA, int dirB, int velA, int velB) {
  // Control del sentido del motor izquierdo
  digitalWrite(in1A, dirA == 1 ? LOW : HIGH);  
  digitalWrite(in2A, dirA == 1 ? HIGH : LOW);
  
  // Control del sentido del motor derecho
  digitalWrite(in1B, dirB == 1 ? LOW : HIGH);  
  digitalWrite(in2B, dirB == 1 ? HIGH : LOW);
  
  // Control de la velocidad
  analogWrite(enaA, velA);
  analogWrite(enbA, velB);
}

// Función para detener el carrito
void detener() {
  digitalWrite(in1A, LOW);
  digitalWrite(in2A, LOW);
  digitalWrite(in1B, LOW);
  digitalWrite(in2B, LOW);
}

// Función para medir la distancia con el sensor ultrasónico
unsigned int medirDistancia() {
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          // Enviar pulso de 10us
  digitalWrite(Trigger, LOW);
  
  tiempo = pulseIn(Echo, HIGH);   // Obtener el tiempo del pulso de vuelta
  return tiempo / 58;             // Convertir tiempo a distancia en cm
}

void loop() {
  distanciaActual = medirDistancia();
  
  if (distanciaActual > 30) {
    // No hay obstáculo, avanzar y resetear contador de giros
    mover(-1, -1, 255, 255);  // Avanzar a velocidad máxima
    girosDerecha = 0;  // Resetear contador de giros
  } else {
    // Hay obstáculo, girar a la derecha
    mover(1, 0, 255, 255);  // Girar a la derecha
    delay(500);  // Girar por medio segundo
    girosDerecha++;  // Aumentar el contador de giros

    if (girosDerecha >= 4) {
      // Si ya ha girado 4 veces a la derecha, retroceder
      mover(1, 1, 255, 255);  // Retroceder a velocidad máxima
      delay(1000);  // Retroceder por 1 segundo
      girosDerecha = 0;  // Resetear contador de giros después de retroceder
    }
  }

  delay(100); // Pequeña pausa antes de la siguiente verificación
}


