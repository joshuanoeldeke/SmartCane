#ifndef SERIAL_OUTPUT_H
#define SERIAL_OUTPUT_H

#include "Schedule.h"

#include "StepGyro.h"
#include "Distancer.h"

#define LOGGING_TIME 250

class SerialLogger : public Schedule {
private:
    StepGyro& m_stepGyro;
    Distancer& m_distancer;
public:
    SerialLogger(StepGyro& stepGyro, Distancer& dist) : m_stepGyro{stepGyro}, m_distancer{dist} {
      
    }
    void init() {
      schedule(LOGGING_TIME);
    }
    virtual void run() override {
      Serial.print("!steps:");
      Serial.println(m_stepGyro.getSteps());
      Serial.print("!km:");
      Serial.println(m_stepGyro.getKms());
      Serial.print("!distance:");
      Serial.println(m_distancer.getDistance());

      schedule(LOGGING_TIME);
    }
};


#endif