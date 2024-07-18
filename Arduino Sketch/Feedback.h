#ifndef AD657757_AB71_4428_92F7_378071D32D2F
#define AD657757_AB71_4428_92F7_378071D32D2F

#include "Log.h"

#include "Buzzer.h"
#include "Schedule.h"

class ObstacleFeedback : public Schedule {
private:
    Buzzer& m_buzzer;
    uint16_t m_durationOn, m_maxDurationOff;
    bool m_isBlocked;
    float m_relTime;
public:
    ObstacleFeedback(Buzzer& buzzer, uint16_t durationOn, uint16_t maxDurationOff) : m_buzzer{buzzer},
            m_durationOn{durationOn}, m_maxDurationOff{maxDurationOff}, m_isBlocked{false}, m_relTime{0} {}
public:
    void start(float relTime) {
        if (!m_isBlocked) {
            m_isBlocked = true;
            m_relTime = relTime;
            m_buzzer.activate();
            schedule(m_durationOn);
        }
    }
    virtual void run() override {
        if (m_isBlocked && m_buzzer.isActive()) {
            // Buzzer has played. Now mute it for some time to get
            // pings with a certain frequency. The frequency is determined
            // by the pause time calculated from the max pause time and
            // the relative time given at start()
            m_buzzer.deactivate();
            schedule((uint16_t) m_maxDurationOff * m_relTime);
        } else {
            // Buzzer has played AND paused. Unblock feedback for next obstacle iteration.
            LOG("Buzzer played for: ") LOGLN(m_maxDurationOff * m_relTime)
            m_isBlocked = false;
        }
    }
};

class CalibrationFeedback : public Schedule {
private:
    Buzzer& m_buzzer;
    uint8_t m_iCalib;
    uint16_t m_durationOn, m_durationOff;
public:
    CalibrationFeedback(Buzzer& buzzer, uint16_t durationOn, uint16_t durationOff) : m_buzzer{buzzer}, m_iCalib{0},
            m_durationOn{durationOn}, m_durationOff{durationOff} {}
public:
    void start(uint8_t level) {
        if (level > 0) {
            m_iCalib = level;
            m_buzzer.activate();
            schedule(m_durationOn);
        }
    }
    // Override run method - gets called at the end of the schedule
    virtual void run() {
        if (m_buzzer.isActive()) {
            m_buzzer.deactivate();
            schedule(m_durationOff);
        } else {
            start(m_iCalib - 1);
        }
    }
};

#endif /* AD657757_AB71_4428_92F7_378071D32D2F */
