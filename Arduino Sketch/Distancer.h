#ifndef E873DC66_592D_4DAE_BB3C_0956A8480267
#define E873DC66_592D_4DAE_BB3C_0956A8480267

#include "Log.h"

#include <NewPing.h>

class Distancer {
private:
    NewPing m_sonar;
    int m_triggerPin, m_echoPin;
    uint16_t m_maxDistance;
    float m_distance;
public:
    Distancer(int triggerPin, int echoPin, uint16_t maxDistance) : m_maxDistance{maxDistance}, m_triggerPin{triggerPin},
            m_echoPin{echoPin}, m_sonar{NewPing(triggerPin, echoPin, maxDistance)}, m_distance{0} {

    }
public:
    void loop(float temp = 22.0) {
        // Calculate speed of sound based on temperature
        float speedOfSound = 331.3 + 0.606 * temp;
        unsigned int duration = m_sonar.ping();
        m_distance = (duration / 2.0) * (speedOfSound / 10000.0);
    }
    float getDistance() {
        return m_distance;
    }
    float getRelDistance() {
        float relDistance = m_distance ? m_distance / m_maxDistance : 0;
        LOG("Rel. distance: ") LOGLN(relDistance)
        return relDistance;
    }
    void calibrate(uint16_t maxDistance) {
        m_maxDistance = maxDistance;
        m_sonar = NewPing(m_triggerPin, m_echoPin, m_maxDistance);
        Serial.print("!max_distance:");
        Serial.println(m_maxDistance);
    }
};

#endif /* E873DC66_592D_4DAE_BB3C_0956A8480267 */