#ifndef A232C979_987D_4118_916B_C07BDF677309
#define A232C979_987D_4118_916B_C07BDF677309

#include "Schedule.h"

/**
 * The headlight class extends schedule.
 * When triggered, it makes the led flash n times
 * for a specified amount of time.
 */
class Headlight : public Schedule {
private:
    int m_pin;
    uint16_t m_durationOn, m_durationOff;
    uint8_t m_nFlashes;
    bool m_isEmitting;
public:
    Headlight(int pin, uint16_t durationOn, uint16_t durationOff) : m_pin{pin}, m_durationOn{durationOn}, m_durationOff{durationOff},
            m_nFlashes{0}, m_isEmitting{false} {}
    void init() {
        pinMode(m_pin, OUTPUT);
    }
public:
    void flash(uint8_t nFlashes = 1) {
        if (nFlashes > 0) {
            m_nFlashes = nFlashes;
            activateLight();
            schedule(m_durationOn);
        }
    }
    // Override run method - gets called at the end of the schedule
    virtual void run() {
        if (m_isEmitting) {
            deactivateLight();
            schedule(m_durationOff);
        } else {
            flash(m_nFlashes - 1);
        }
    }
    virtual void stop() override {
        Schedule::stop();
        deactivateLight();
    }
private:
    void activateLight() {
        digitalWrite(m_pin, HIGH);
        m_isEmitting = true;
    }
    void deactivateLight() {
        digitalWrite(m_pin, LOW);
        m_isEmitting = false;
    }
};

#endif /* A232C979_987D_4118_916B_C07BDF677309 */