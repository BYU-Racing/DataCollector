#include <Arduino.h>
#include <FlexCAN_T4.h>
#include <DataCollector.h>
#include <DigitalSensor.h>
#include <AnalogSensor.h>

constexpr bool THROTTLE_1_CRITICALITY = true;
constexpr uint8_t THROTTLE_1_PIN = 39;

constexpr bool THROTTLE_2_CRITICALITY = true;
constexpr uint8_t THROTTLE_2_PIN = 40;

constexpr uint32_t THROTTLE_INTERVAL = 10;

AnalogSensor throttle1 = AnalogSensor(ReservedIDs::Throttle1Position, THROTTLE_1_CRITICALITY, THROTTLE_1_PIN, THROTTLE_INTERVAL);
AnalogSensor throttle2 = AnalogSensor(ReservedIDs::Throttle2Position, THROTTLE_1_CRITICALITY, THROTTLE_2_PIN, THROTTLE_INTERVAL);

constexpr bool BRAKE_CRITICALITY = true;
constexpr uint8_t BRAKE_PIN = 21;
constexpr uint32_t BRAKE_INTERVAL = 10;

AnalogSensor brake = AnalogSensor(ReservedIDs::BrakePressure, BRAKE_CRITICALITY, BRAKE_PIN, BRAKE_INTERVAL);

constexpr bool SWITCH_CRITICALITY = true;
constexpr uint8_t SWITCH_PIN = 38;
constexpr uint32_t SWITCH_INTERVAL = 10;

DigitalSensor startSwitch = DigitalSensor(ReservedIDs::StartSwitch, SWITCH_CRITICALITY, SWITCH_PIN, SWITCH_INTERVAL);

constexpr size_t NUM_SENSORS = 4;
Sensor* SENSORS[] = {
    &throttle1,
    &throttle2,
    &brake,
    &startSwitch
};
FlexCAN_T4<CAN1, RX_SIZE_256> motorCAN;
FlexCAN_T4<CAN2, RX_SIZE_256> dataCAN;

DataCollector dc = DataCollector(ReservedIDs::DCF, NUM_SENSORS, SENSORS);

constexpr uint32_t BAUD_RATE = 112500;

void setup() {
    Serial.begin(BAUD_RATE);
    motorCAN.begin();
    motorCAN.setBaudRate(BAUD_RATE);
    dataCAN.begin();
    dataCAN.setBaudRate(BAUD_RATE);
    dc.begin(&motorCAN, &dataCAN);
}

void loop() {
    dc.checkSensors();
}
