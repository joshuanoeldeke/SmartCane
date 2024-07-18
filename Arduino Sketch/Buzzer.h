#ifndef CCDA9A23_9E7C_4AE2_9B43_8C1A6CE04D59
#define CCDA9A23_9E7C_4AE2_9B43_8C1A6CE04D59

class Buzzer {
private:
    int m_pin;
    bool m_isActive;
public:
    Buzzer(int pin) : m_pin{pin} {}
    void init() {
        pinMode(m_pin, OUTPUT);
    }
public:
    void activate() {
        m_isActive = true;
        digitalWrite(m_pin, HIGH);
    }
    void deactivate() {
        m_isActive = false;
        digitalWrite(m_pin, LOW);
    }
    inline bool isActive() const {return m_isActive;}
};

#endif /* CCDA9A23_9E7C_4AE2_9B43_8C1A6CE04D59 */
