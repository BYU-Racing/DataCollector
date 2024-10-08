#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include <Arduino.h>
#include <FlexCAN_T4.h>
#include <Sensor.h>

/** Macro for the DataCollector template */
#define DC_TP template <\
    CAN_DEV_TABLE _bus, \
    FLEXCAN_RXQUEUE_TABLE _rxSize = RX_SIZE_16, \
    FLEXCAN_TXQUEUE_TABLE _txSize = TX_SIZE_16\
    >
/** Macro for help implementing the DataCollector template */
#define DC_IMPL DataCollector<_bus, _rxSize, _txSize>

constexpr uint32_t HEALTH_CHECK_REQ_ID = 100;
constexpr uint8_t HEALTH_CHECK_RES_LEN = 1;

/**
 * Enum representing the current DataCollector health.
 *
 * This value represents the status of the *least* healthy sensor.
 */
const enum healthStatus : uint8_t
{
    // All sensors are functioning properly
    HEALTHY = 3,
    // 1 or more *non-critical* sensor(s) are not functioning properly
    UNHEALTHY = 2,
    // 1 or more *critial* sensor(s) are not functioning properly
    FAILURE = 1,
    // Used for default initialization
    UNKNOWN = 0,
};

DC_TP
class DataCollector {
    uint32_t id = 0;
    size_t numSensors = 0;
    Sensor** sensors = nullptr;
    FlexCAN_T4<_bus, _rxSize, _txSize>* can = nullptr;
    CAN_message_t healthMsg;
    healthStatus health = UNKNOWN;
    bool debug = false;
    /**
     * Get sensor data and send over CAN line.
     * sensor->debugPrint(CAN_messate_t) is called if debug is true.
     * @param sensor Sensor to read from
     */
    void read(Sensor* sensor);
    /**
     * Get a sensors health and ability to be read form
     * @param sensor Sensor to update the DataCollector health with
     * @return true if sensor can be read from, false otherwise
     */
    bool healthCheck(const Sensor* sensor);
public:
    /** begin() must be called after initialization for CAN functionality */
    DataCollector(uint32_t id, size_t numSensors, Sensor** sensors, bool debug = false);
    ~DataCollector() = default; // No pointers are owned, no `delete` necessary
    /**
     * Poll all of the sensors given to the DataCollector.
     * Calls `healthCheck()` if a health check request is received via CAN
     */
    void checkSensors();
    /**
     * Assigns a CAN line to read/write to/from.
     * Must be called prior to checkSensors() or healthCheck().
     */
    void begin(FlexCAN_T4<_bus, _rxSize, _txSize>* can);
    /** Sends a health check message to the CAN line */
    void healthCheck();
};

#include "DataCollector.tpp" // Include the template implementations

#endif // DATACOLLECTOR_H
