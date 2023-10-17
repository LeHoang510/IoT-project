#pragma once
#include <WString.h>



class EnvSensor
{
    protected:

    float m_temp;
    float m_hum;
    float m_pressure;
    String m_mac;

    public:
    EnvSensor();
    EnvSensor(const String &mac, float temp, float hum, float pressure);
    EnvSensor(const EnvSensor &c);
    ~EnvSensor();

    float getTemp() const { return m_temp;}
    float getHum() const { return m_hum;}
    float getPressure() const { return m_pressure;}
    String getMac() const { return m_mac;}

    void setTemp(float temp) { m_temp = temp;}
    void setHum(float hum){m_hum = hum;}
    void setPressure(float pressure){m_pressure = pressure;}
    void setMac(const String & mac){m_mac = mac;}


    bool operator==(const EnvSensor & rhs) const;
    bool operator<(const EnvSensor & rhs) const;
    EnvSensor& operator=(const EnvSensor & rhs);
    

};