
// Define debug for debug output printed with LOG
// #define DEBUG

#include "Log.h"
#include "Headlight.h"
#include "BouncingJoystick.h"
#include "StepGyro.h"
#include "Distancer.h"
#include "DistanceCalibrator.h"
#include "Buzzer.h"
#include "Feedback.h"
#include "SerialLogger.h"

Headlight light_left(6, 1000, 400);
Headlight light_right(5, 1000, 400);
BouncingJoystick joystick(A0, A1, 2);
Distancer distancer(9, 10, 400);
DistanceCalibrator calibrator(distancer);
StepGyro stepGyro = StepGyro();
Buzzer buzzer = Buzzer(3);
CalibrationFeedback calibrationFeedback(buzzer, 200, 300);
ObstacleFeedback obstacleFeedback(buzzer, 30, 1000);
SerialLogger logger(stepGyro, distancer);

void setup() {
    Serial.begin(115200);
    while (!Serial);

    joystick.init();
    stepGyro.init();
    buzzer.init();
    light_left.init();
    light_right.init();
    logger.init();
}

void loop() {
    light_left.loop();
    light_right.loop();
    calibrationFeedback.loop();
    obstacleFeedback.loop();
    stepGyro.loop();
    joystick.trackPressure();
    logger.loop();
    distancer.loop(stepGyro.getTemperature());

    Direction direction = joystick.getPulse();
    switch (direction) {
        case Direction::LEFT:
            light_left.flash(5);
            light_right.stop();
            Serial.print("!blink:");
            Serial.println("left");
            break;
        case Direction::RIGHT:
            light_right.flash(5);
            light_left.stop();
            Serial.print("!blink:");
            Serial.println("right");
            break;
        case Direction::UP:
            calibrator.calibrate(1);
            if (joystick.getToggle()) {
                calibrationFeedback.start(calibrator.getLevel());
            }
            break;
        case Direction::DOWN:
            calibrator.calibrate(-1);
            if (joystick.getToggle()) {
                calibrationFeedback.start(calibrator.getLevel());
            }
            break;
        default:
            break;
    }

    float relDistance = calibrator.getRelDistance();
    if (relDistance > 0 && joystick.getToggle()) {  // Only send feedback when it is not disabled by hte joystick button toggle
        obstacleFeedback.start(relDistance);        // Gets suppressed if there was already feedback a while ago
    }
}