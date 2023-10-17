#include "EnvSensor.hpp"


EnvSensor::EnvSensor():m_mac(""),m_temp(0), m_hum(0), m_pressure(0){

}
EnvSensor::EnvSensor(const String &mac, float temp, float hum, float pressure):m_mac(mac),m_temp(temp), m_hum(hum), m_pressure(pressure){

}
EnvSensor::EnvSensor(const EnvSensor &c):m_mac(c.m_mac),m_temp(c.m_temp), m_hum(c.m_hum), m_pressure(c.m_pressure){

}
EnvSensor::~EnvSensor(){

}

bool EnvSensor::operator==(const EnvSensor & rhs) const {
    return (m_mac.compareTo(rhs.m_mac) == 0);
}

bool EnvSensor::operator<(const EnvSensor & rhs) const {
    return (m_mac.compareTo(rhs.m_mac) < 0);
}

EnvSensor& EnvSensor::operator=(const EnvSensor & rhs){
    m_mac = rhs.m_mac;
    m_temp = rhs.m_temp;
    m_pressure = rhs.m_pressure;
    m_hum = rhs.m_hum;
    return *this;
}