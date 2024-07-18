#ifndef C247270E_9DA3_4EFF_B494_CBC5C615504C
#define C247270E_9DA3_4EFF_B494_CBC5C615504C

#define MAX_LONG 4294967295

class Schedule {
private:
    unsigned long m_scheduled;
public:
    Schedule() : m_scheduled{MAX_LONG} {}
    void schedule(long ms) {
        m_scheduled = millis() + ms;
    }
    virtual void loop() {
        if (millis() > m_scheduled) {
            m_scheduled = MAX_LONG;
            run();
        }
    }
    virtual void run() {
        Serial.println("Scheduled event.");
    }
    virtual void stop() {
        m_scheduled = MAX_LONG;
    }
};

#endif /* C247270E_9DA3_4EFF_B494_CBC5C615504C */
