#include <Arduino.h>
#include <Servo.h>

int min = 340;
int max = 2480;

int led = 13;

struct LegServos
{
    Servo s0, s1, s2;
};



LegServos l0, l1;
void setup() {
    // put your setup code here, to run once:
    l1.s0.attach(9);
    l1.s1.attach(10);
    l1.s2.attach(11);

    l1.s2.write(180);
    l0.s0.attach(8);
    l0.s1.attach(7);
    l0.s2.attach(6);

    pinMode(led, OUTPUT);
}

int d = 1300;

void loop() {
    // l0.s1.write(180);
    // delay(d);
    // l0.s1.write(00);
    // delay(d);
    //servo.write(30);
    digitalWrite(led, HIGH);
    delay(250);
    digitalWrite(led, LOW);
    delay(250);
    // put your main code here, to run repeatedly:
}