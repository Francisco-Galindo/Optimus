#define trig 9
#define echo 10

int duracion;
int distancia;

void setup() {
	pinMode(trig, OUTPUT);
	pinMode(echo, INPUT);
	Serial.begin(9600);
}

void loop() {

	digitalWrite(trig, HIGH);
	delay(1);
	digitalWrite(trig, LOW);

	duracion = pulseIn(echo, HIGH);
	distancia = duracion / 58.2;

	Serial.println(distancia);
	delay(30);

}
