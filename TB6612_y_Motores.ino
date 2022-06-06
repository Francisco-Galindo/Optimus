//COMENTARIOS DEL CIRCUITO:
//
//1.- VM va con las pilas y VCC a 5V del Arduino, las tierras del driver van todas a la tierra de las pilas y las tierras de los sensores a la tierra del Arduino.
//2.- Si no jala, checar que no haya HIGH-HIGH o checar bien que sea "ina1 e ina2".
//3.- Estado inicial es 1, si detecta 0, cambia de estado.
//4.- ao1 al positivo, ao2 al negativo y bo1 al negativo, bo2 al positivo, para que avancen igual y en el código sea ia1-HIGH ib1-HIGH.
//5.- No mover el Stand-By del void setup.
//6.- Identificar bien el sensor trasero y el delantero.
//7.- Verificar que los sensores esten derechos.
//8.- Los tiempos se ajustan en cada subrutina (solo si es necesario, pues el robot avanza hasta que no detecte más el borde de la cancha).




int sensordelantero = A0;
int lecdelantero = 0;

int sensortrasero = A1;
int lectrasero = 0;

int duracion;
int distancia;

const int trig = 9;
const int echo = 10;

const int pwma = 2;
const int pwmb = 8;

const int stby = 5;

const int ain1 = 6;
const int ain2 = 7;

const int bin1 = 4;
const int bin2 = 3;


void setup() {

  pinMode(pwma, OUTPUT);
  pinMode(pwmb, OUTPUT);

  pinMode(stby, OUTPUT);

  pinMode(ain1, OUTPUT);
  pinMode(ain2, OUTPUT);

  pinMode(bin1, OUTPUT);
  pinMode(bin2, OUTPUT);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  digitalWrite(stby, HIGH);

  Serial.begin(9600);
  delay(5000);

}

void loop() {


  /* RUTINA PRINCIPAL */
  while(digitalRead(sensordelantero) == 1 && digitalRead(sensortrasero) == 1 && distancia > 60)

    digitalWrite(trig, HIGH);
    delay(1);
    digitalWrite(trig, LOW);

    duracion = pulseIn(echo, HIGH);
    distancia = duracion / 58.2;

    Serial.println(distancia);
    delay(30);

    Serial.print("Sensor Delantero: ");
    Serial.print(lecdelantero);
    Serial.print("    Sensor Trasero:");
    Serial.print(lectrasero);
    Serial.print("\n");
    delay(50);

    principal();
  }

  if (distancia <= 60 && digitalRead(sensordelantero) == 1) {
    adelante();
  }

  if(digitalRead(sensordelantero) != 1) {
    extremodelantero();
  }
  if(digitalRead(sensortrasero) != 1) {
    extremotrasero();
  }
  if(digitalRead(sensordelantero) != 1 && digitalRead(sensortrasero) != 1) {
    apagado();
  }

}


void principal() {

  analogWrite(pwma, 100);
  analogWrite(pwmb, 100);

  digitalWrite(ain1, HIGH);
  digitalWrite(ain2, LOW);

  digitalWrite(bin1, LOW);
  digitalWrite(bin2, HIGH);

}

void adelante() {

  analogWrite(pwma, 100);
  analogWrite(pwmb, 100);

  digitalWrite(ain1, HIGH);
  digitalWrite(ain2, LOW);

  digitalWrite(bin1, HIGH);
  digitalWrite(bin2, LOW);

}


void extremodelantero() {

  analogWrite(pwma, 50);
  analogWrite(pwmb, 50);
  digitalWrite(ain1, LOW);
  digitalWrite(ain2, HIGH);
  digitalWrite(bin1, LOW);
  digitalWrite(bin2, HIGH);

}


void extremotrasero() {

  analogWrite(pwma, 100);
  analogWrite(pwmb, 100);
  digitalWrite(ain1, HIGH);
  digitalWrite(ain2, LOW);
  digitalWrite(bin1, HIGH);
  digitalWrite(bin2, LOW);

}

void apagado() {

  analogWrite(pwma, 0);
  analogWrite(pwmb, 0);
  digitalWrite(ain1, LOW);
  digitalWrite(ain2, LOW);
  digitalWrite(bin1, LOW);
  digitalWrite(bin2, LOW);

}
