#ifndef B0C19477_CF70_4591_8361_FA5AC3DBFE40
#define B0C19477_CF70_4591_8361_FA5AC3DBFE40

#include "Log.h"

/**
 * Direction enum
 * 
 * Note that you can
 * - check if a direction is specified: if(direction)
 * - check if the direction is on the y axis: if(direction >= 3)
 */
enum Direction : uint8_t {
    NONE = 0, LEFT = 1, RIGHT = 2, UP = 3, DOWN = 4
};

/**
 * Wrapper class around a joystick input - with a twist!
 * The bouncing joystick can determine the direction in which the
 * joytick is held (LEFT, RIGH, UP, DOWN) and only sends one pulse.
 * So you have to move the joystick back to the middle before getting
 * a second input for the same direction.
 */
class BouncingJoystick {
private:
    int m_pinX, m_pinY, m_pinPress;
    bool m_isReceivingX, m_isReceivingY, m_isReceivingPress, m_buttonToggle;
public:
    BouncingJoystick(int pinX, int pinY, int pinPress) : m_pinX{pinX}, m_pinY{pinY}, m_pinPress{pinPress},
            m_isReceivingX{true}, m_isReceivingY{true}, m_isReceivingPress{true}, m_buttonToggle{true} {}
    void init() {
        pinMode(m_pinX, INPUT);
        pinMode(m_pinY, INPUT);
        pinMode(m_pinPress, INPUT_PULLUP);
    }
public:
    /**
     * Call this function in the loop.
     * Returns the direction of the current pulse.
     * Doesn't return the same direction twice
     * unless the joystick entered the neutral position again.
     */
    Direction getPulse() {
        int x = analogRead(m_pinX);
        int y = analogRead(m_pinY);
        
        LOG("Joystick ** X : ") LOG(x) LOG(" | Y : ") LOGLN(y)

        if (isNeutral(x)) {
            m_isReceivingX = true;
        }
        if (isNeutral(y)) {
            m_isReceivingY = true;
        }

        if (m_isReceivingX) {
            if (isLeft(x)) {
                m_isReceivingX = false;
                return Direction::LEFT;
            }
            if (isRight(x)) {
                m_isReceivingX = false;
                return Direction::RIGHT;
            }
        }
        if (m_isReceivingY) {
            if (isUp(y)) {
                m_isReceivingY = false;
                return Direction::UP;
            }
            if (isDown(y)) {
                m_isReceivingY = false;
                return Direction::DOWN;
            }
        }

        return Direction::NONE;
    }
    /**
     * Call this function in the loop.
     * Detects whether the joystick button is newly pressed.
     */
    bool trackPressure() {
        bool released = digitalRead(m_pinPress);
        LOG("Joystick ** Pressure : ") LOGLN(released)
        if (released) {
            m_isReceivingPress = true;
        } else if (m_isReceivingPress) {
            m_isReceivingPress = false;
            toggleButton();
            return true;
        }
        return false;
    }
    inline bool getToggle() const {
        LOG("Joystick ** button toggle : ") LOGLN(m_buttonToggle)
        return m_buttonToggle;
    }
private:
    inline bool isNeutral(int v) {return v > 400 && v < 600;}
    inline bool isLeft(int x) {return x < 200;}
    inline bool isRight(int x) {return x > 800;}
    inline bool isUp(int y) {return y < 200;}
    inline bool isDown(int y) {return y > 800;}
    void toggleButton() {
        m_buttonToggle = !m_buttonToggle;
    }
};

#endif /* B0C19477_CF70_4591_8361_FA5AC3DBFE40 */
