/*
 *
 * COMENTARIOS DEL CIRCUITO:
 *
 * 1.- VM va con las pilas y VCC a 5V del Arduino, las tierras del driver van
 * todas a la tierra de las pilas y las tierras de los sensores a la tierra del
 * Arduino.
 *
 * 2.- Si no jala, checar que no haya HIGH-HIGH o checar bien que sea
 * "ina1 e ina2".
 *
 * 3.- Estado inicial es 1, si detecta 0, cambia de estado.
 *
 * 4.- ao1 al positivo, ao2 al negativo y bo1 al negativo, bo2 al positivo,
 * para que avancen igual y en el código sea ia1-HIGH ib1-HIGH.
 *
 * 5.- No mover el Stand-By del void setup.
 *
 * 6.- Identificar bien el sensor trasero y el delantero.
 *
 * 7.- Verificar que los sensores esten derechos.
 *
 * 8.- Los tiempos se ajustan en cada subrutina (solo si es necesario, pues el
 * robot avanza hasta que no detecte más el borde de la cancha).

 */

#define DISTANCIA_MIN 40

int lecdelantero = 0;
int lectrasero = 0;
unsigned long inicio_busqueda;
bool buscando = false;

byte distancia;
int duracion;

const int sens_del_izq = A0;
int del_izq = 0;

const int sens_del_der = A1;
int del_der = 0;

const int sens_tras_izq = A2;
int tras_izq = 0;

const int sens_tras_der = A3;
int tras_der = 0;

const byte trig = 9;
const byte echo = 10;

const byte pwma = 2;
const byte pwmb = 8;

const byte stby = 5;

/* Izquierda */
const byte ain1 = 6;
const byte ain2 = 7;

/* Derecha */
const byte bin1 = 4;
const byte bin2 = 3;


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

	pinMode(sens_del_izq, INPUT);
	pinMode(sens_del_der, INPUT);
	pinMode(sens_tras_izq, INPUT);
	pinMode(sens_tras_der, INPUT);

	digitalWrite(stby, HIGH);

	Serial.begin(9600);
	delay(5000);
}

void loop() {

	if (se_sale()) {

		if (en_borde_del_izq() && en_borde_del_der()) {
			regresar_borde();
		} else if (en_borde_del_izq()) {
			regresar_borde_izq();
		} else if (en_borde_del_der()) {
			regresar_borde_der();
		} else {
			avanzar();
		}

	} else {

		if (algo_enfrente()) {
			buscando = false;
			perseguir();
		} else {
			if (!buscando) {
				buscando = true;
				inicio_busqueda = millis();
			}
			tornado();
		}

	}


	delay(15);
}

void tornado() {
	const long int ahora = millis();
	analogWrite(pwma, 100);
	analogWrite(pwmb, 100);
	if (ahora - inicio_busqueda < 5000) {
		girar_llanta_izq(HIGH);
		girar_llanta_der(LOW);
	} else if (ahora - inicio_busqueda < 6000) {
		girar_llanta_izq(HIGH);
		girar_llanta_der(HIGH);
	} else {
		buscando = false;
	}
}

void perseguir() {

	byte dir_izq;
	byte dir_der;

	if (distancia_izq() && !distancia_der()) {
		dir_izq = HIGH;
		dir_der = LOW;
	} else if (!distancia_izq() && distancia_der) {
		dir_izq = LOW;
		dir_der = HIGH;
	} else {
		dir_izq = HIGH;
		dir_der = HIGH;
	}

	analogWrite(pwma, 100 * dir_izq);
	analogWrite(pwmb, 100 * dir_der);

	girar_llanta_izq(dir_izq);
	girar_llanta_der(dir_der);
}

/* Devuelve la distancia detectada por el sensor ultrasónico */
byte distancia_sonic() {

	digitalWrite(trig, HIGH);
	delay(1);
	digitalWrite(trig, LOW);
	delay(30);

	duracion = pulseIn(echo, HIGH);

	return duracion / 58.2;
}

bool distancia_der() {
	return false;
}

bool distancia_izq() {
	return false;
}

bool algo_enfrente() {
	bool izq = distancia_izq();
	bool der = distancia_der();
	bool centro = distancia_sonic() < DISTANCIA_MIN;
	return izq || der || centro;
}

bool en_borde_del_izq() {
	return en_borde(sens_del_izq);
}

bool en_borde_del_der() {
	return en_borde(sens_del_izq);
}

bool en_borde_tras_izq() {
	return en_borde(sens_tras_izq);
}

bool en_borde_tras_der() {
	return en_borde(sens_tras_der);
}

bool en_borde(byte pin_sensor) {
	return digitalRead(pin_sensor);
}

bool se_sale() {
	return en_borde_del_izq() ||
		en_borde_del_der() ||
		en_borde_tras_izq() ||
		en_borde_tras_der();
}

void regresar_borde() {
	analogWrite(pwma, 100);
	analogWrite(pwmb, 100);

	girar_llanta_izq(false);
	girar_llanta_der(false);
	delay(300);
}

void regresar_borde_izq() {
	regresar_borde();

	analogWrite(pwma, 100);
	analogWrite(pwmb, 100);

	girar_llanta_izq(true);
	girar_llanta_der(false);
	delay(300);
}

void regresar_borde_der() {
	regresar_borde();

	analogWrite(pwma, 100);
	analogWrite(pwmb, 100);

	girar_llanta_izq(false);
	girar_llanta_der(true);
	delay(300);
}

void avanzar() {
	analogWrite(pwma, 100);
	analogWrite(pwmb, 100);

	girar_llanta_izq(true);
	girar_llanta_der(true);
	delay(100);
}

void girar_llanta_izq(bool adelante) {
	digitalWrite(ain1, adelante);
	digitalWrite(ain2, !adelante);
}

void girar_llanta_der(bool adelante) {
	digitalWrite(bin1, adelante);
	digitalWrite(bin2, !adelante);
}
