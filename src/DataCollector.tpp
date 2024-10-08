#ifndef DATACOLLECTOR_TPP
#define DATACOLLECTOR_TPP

#include "DataCollector.h"
#include <SensorData.h>

DC_TP DC_IMPL::DataCollector(uint32_t id, size_t numSensors, Sensor** sensors, const bool debug)
{
    this->id = id;
    this->numSensors = numSensors;
    this->sensors = sensors;
    this->debug = debug;
}

DC_TP void DC_IMPL::begin(FlexCAN_T4<_bus, _rxSize, _txSize>* can)
{
    this->can = can;
    health = HEALTHY;
}

DC_TP void DC_IMPL::read(Sensor* sensor)
{
    const SensorData data = sensor->read();
    const size_t msgCount = data.getMsgCount();
    const CAN_message_t* msgs = data.getMsgs();
    for (size_t msgIndex = 0; msgIndex < msgCount; msgIndex++)
    {
        const CAN_message_t msg = msgs[msgIndex];
        if (debug) { sensor->debugPrint(msg); }
        can->write(msg);
    }
}

DC_TP bool DC_IMPL::healthCheck(const Sensor* sensor)
{
    if (sensor->healthCheck())
    {
        return true;
    }
    // Sensor failed the healthCheck and health status must change
    if (sensor->isCritical())
    {
        health = FAILURE;
    } else if (health != FAILURE) // Don't override FAILURE with a "healthier" state
    {
        health = UNHEALTHY;
    }
    return false;
}

DC_TP void DC_IMPL::healthCheck()
{
    healthMsg.id = id;
    healthMsg.len = HEALTH_CHECK_RES_LEN;
    healthMsg.buf[0] = health;
    can->write(healthMsg);
}

DC_TP void DC_IMPL::checkSensors() {
    for (size_t sensorIndex = 0; sensorIndex < numSensors; sensorIndex++)
    {
        if (Sensor* sensor = sensors[sensorIndex]; healthCheck(sensor) && sensor->ready())
        {
            read(sensor);
        }
    }
    if (can.read(healthMsg) && healthMsg.id == HEALTH_CHECK_REQ_ID)
    {
        healthCheck();
    }
}

#endif // DATACOLLECTOR_TPP
