#ifndef A919C22B_8B39_4E0B_8F3A_31261522E8BA
#define A919C22B_8B39_4E0B_8F3A_31261522E8BA

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

// Sensitivity scale factors
#define m_accel_SCALE_FACTOR_2G 16384.0
#define GYRO_SCALE_FACTOR_250DPS 131.0
#define CALIBRATION_CYCLES 6

// Factors for calculating steps
#define STEP_THRESHOLD 15000
#define STRIDE_LENGTH 0.78      // Average stride length in meters (adjust as needed)

/**
 * Human-Readable data from the gyro and acceleration sensors.
 * Also detects steps and kilometers walked.
 */
struct GyroData {
    float ax, ay, az, gx, gy, gz;
};

class StepGyro {
private:
    MPU6050 m_mpu;
    int16_t ax, ay, az;
    int16_t last_ax, last_ay, last_az;
    long m_stepCount;
    float m_distanceWalked ;
public:
    StepGyro() : last_ax{0}, last_ay{0}, last_az{0}, m_stepCount{0}, m_distanceWalked{0} {}
    void init() {
        Wire.begin();
        m_mpu.initialize();
        if (!m_mpu.testConnection()) {
            Serial.println("MPU6050 connection failed");
            while (1);
        }
        // Perform automatic calibration
        m_mpu.CalibrateAccel(CALIBRATION_CYCLES);
        m_mpu.CalibrateGyro(CALIBRATION_CYCLES);
        m_mpu.PrintActiveOffsets();
    }
public:
    void loop() {
        calculateSteps();
    }
public:
    float getTemperature() {
        int16_t rawTemp = m_mpu.getTemperature();
        return (float) (rawTemp / 340.0 + 36.53);
    }
    long getSteps() const {
      return m_stepCount;
    }
    float getKms() const {
      return m_distanceWalked;
    }
private:
    void calculateSteps() {
        m_mpu.getAcceleration(&ax, &ay, &az);

        last_ax = 0;
        last_ay = 0;
        last_az = 0;

        // Simple peak detection for step counting
        if ((abs(ax - last_ax) > STEP_THRESHOLD) || (abs(ay - last_ay) > STEP_THRESHOLD) || (abs(az - last_az) > STEP_THRESHOLD)) {
            m_stepCount++;
            m_distanceWalked = (m_stepCount * STRIDE_LENGTH) / 1000.0;      // Convert to kilometers
        }

        last_ax = ax;
        last_ay = ay;
        last_az = az;
    }
};

#endif /* A919C22B_8B39_4E0B_8F3A_31261522E8BA */