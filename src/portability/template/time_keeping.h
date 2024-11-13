#ifndef MOTOR_HEAD_UTILITY_TEST_TIME_KEEPING_H_
#define MOTOR_HEAD_UTILITY_TEST_TIME_KEEPING_H_
#ifndef ARDUINO
    #include <cstdint>

    #ifdef __cplusplus
extern "C" {
    #endif

namespace MotorHead {

    void delay(uint32_t milliseconds) { (void)milliseconds; };
    void delayMicroseconds(uint32_t microseconds) { (void)microseconds; };
    uint32_t millis() { return 0; };

} // namespace MotorHead

    #ifdef __cplusplus
}
    #endif
#endif // ARDUINO
#endif // MOTOR_HEAD_UTILITY_TEST_TIME_KEEPING_H_
