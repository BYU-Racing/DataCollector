#include <Arduino.h>
#include <FlexCAN_T4.h>
#include <DataCollector.h>
#include <DigitalSensor.h>
#include <AnalogSensor.h>
#include <Sensor.h>

constexpr uint32_t THROTTLE_1_ID = 3;
constexpr bool THROTTLE_1_CRITICALITY = true;
constexpr uint8_t THROTTLE_1_PIN = 39;

constexpr uint32_t THROTTLE_2_ID = 4;
constexpr bool THROTTLE_2_CRITICALITY = true;
constexpr uint8_t THROTTLE_2_PIN = 40;

constexpr uint32_t THROTTLE_INTERVAL = 10;

AnalogSensor throttle1 = AnalogSensor(THROTTLE_1_ID, THROTTLE_1_CRITICALITY, THROTTLE_1_PIN, THROTTLE_INTERVAL);
AnalogSensor throttle2 = AnalogSensor(THROTTLE_2_ID, THROTTLE_1_CRITICALITY, THROTTLE_2_PIN, THROTTLE_INTERVAL);

constexpr uint32_t BRAKE_ID = 2;
constexpr bool BRAKE_CRITICALITY = true;
constexpr uint8_t BRAKE_PIN = 21;
constexpr uint32_t BRAKE_INTERVAL = 10;

AnalogSensor brake = AnalogSensor(BRAKE_ID, BRAKE_CRITICALITY, BRAKE_PIN, BRAKE_INTERVAL);

constexpr uint32_t SWITCH_ID = 1;
constexpr bool SWITCH_CRITICALITY = true;
constexpr uint8_t SWITCH_PIN = 38;
constexpr uint32_t SWITCH_INTERVAL = 10;

DigitalSensor startSwitch = DigitalSensor(SWITCH_ID, SWITCH_CRITICALITY, SWITCH_PIN, SWITCH_INTERVAL);

constexpr uint32_t DATACOLLECTOR_ID = 5;
constexpr size_t NUM_SENSORS = 4;
Sensor* SENSORS[] = {
    &throttle1,
    &throttle2,
    &brake,
    &startSwitch
};
constexpr CAN_DEV_TABLE CAN_BUS = CAN0;
constexpr FLEXCAN_RXQUEUE_TABLE RX_SIZE = RX_SIZE_256;
constexpr FLEXCAN_TXQUEUE_TABLE TX_SIZE = TX_SIZE_16;
FlexCAN_T4<CAN_BUS, RX_SIZE, TX_SIZE> dataCan;

DataCollector dc = DataCollector<CAN_BUS, RX_SIZE, TX_SIZE>(DATACOLLECTOR_ID, NUM_SENSORS, SENSORS);

constexpr uint32_t BAUD_RATE = 112500;

void setup() {
    Serial.begin(BAUD_RATE);
    dataCan.begin();
    dataCan.setBaudRate(BAUD_RATE);
    dc.begin(&dataCan);
}

void loop() {
    dc.checkSensors();
}
