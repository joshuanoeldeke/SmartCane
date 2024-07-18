#ifndef C4BCA107_D7D7_4294_A6C6_7A2B8F6E8E09
#define C4BCA107_D7D7_4294_A6C6_7A2B8F6E8E09

#include "Distancer.h"

#define MAX_CALIBRATOR_DISTANCE 100

const uint16_t distances[] = {75, 200, 400};
const uint8_t nDistances = 3;       // Must equal the amount of elements in distances[]

/**
 * Calibrator for the distancer.
 * Calibrates the maximum distance, used by the distancer to improve data
 * quality and calculate the relative distance <max_distance>/<distance>.
 */
class DistanceCalibrator {
private:
    uint8_t m_level;
    Distancer& m_dist;
public:
    DistanceCalibrator(Distancer& dist) : m_level{(int) nDistances/2 + 1}, m_dist{dist} {}
    /**
     * Tries to change the calibration by n steps.
     * Negative steps decrease the calibration, positive steps increase it.
     */
    uint8_t calibrate(int step) {
        m_level = constrain(m_level + step, 1, nDistances);
        m_dist.calibrate(distances[m_level - 1]);
        return m_level;
    }
    /**
     * Returns level of calibration.
     */
    uint8_t getLevel() {
        return m_level;
    }
    float getRelDistance() {
      float distance = m_dist.getDistance();
      if (distance > MAX_CALIBRATOR_DISTANCE) {
        return 0;
      }
      return (distance * m_level) / MAX_CALIBRATOR_DISTANCE;
    }
};

#endif /* C4BCA107_D7D7_4294_A6C6_7A2B8F6E8E09 */
