#include <Arduino.h>
#include <Servo.h>

int min = 340;
int max = 2480;

int led = 13;

struct LegServos
{
    Servo s0, s1, s2;
};

LegServos l0, l1, l2, l3;

void setup() {
    l1.s0.attach(9);
    l1.s1.attach(10);
    l1.s2.attach(11);

    l1.s2.write(163); // straight = 163, 90deg = 111
    l1.s1.write(133); // up = 35, straight = 133

    l0.s0.attach(8);
    l0.s1.attach(7);
    l0.s2.attach(6);

    l0.s2.write(111); // straight = 17, 90deg = 111

    l2.s0.attach(3);
    l2.s1.attach(4);
    l2.s2.attach(5);

    l3.s0.attach(2);
    l3.s1.attach(A5);
    l3.s2.attach(A6);

    pinMode(led, OUTPUT);
}

int d = 1300;

void loop() {
    // l0.s1.write(180);
    // delay(d);
    // l0.s1.write(00);
    // delay(d);

    digitalWrite(led, HIGH);
    delay(250);
    digitalWrite(led, LOW);
    delay(250);
}