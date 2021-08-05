#include <Arduino.h>
#include <Servo.h>

#include "kinematics.hpp"

int min = 340;
int max = 2480;

int led = 13;

struct LegServos
{
    Servo s0, s1, s2;
};

LegServos l0, l1, l2, l3;

void setup() {
    l0.s0.attach(8); // side = 66, forward = 160
    l0.s1.attach(7); // up = 150, straight = 52
    l0.s2.attach(6); // straight = 17, 90deg = 111

    l1.s0.attach(9); // side = 134, backward = 30
    l1.s1.attach(10); // up = 35, straight = 133
    l1.s2.attach(11); // straight = 163, 90deg = 111

    l2.s0.attach(3); // side = 74, backward = 170
    l2.s1.attach(4); // straight = 23, 90deg = 120
    l2.s2.attach(5); // straight = 23, 90deg = 120

    l3.s0.attach(2); // side = 114, forward = 12
    l3.s1.attach(A5); // up = 23, straight = 120
    l3.s2.attach(12); // straight = 148, 90deg = 46

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