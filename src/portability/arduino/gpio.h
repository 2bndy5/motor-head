#ifndef MOTOR_HEAD_UTILITY_ARDUINO_GPIO_H_
#define MOTOR_HEAD_UTILITY_ARDUINO_GPIO_H_
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

namespace MotorHead {

    typedef uint8_t gpio_pin_t;
    const gpio_pin_t NULL_PIN = 0xFF;

    enum Direction : bool
    {
        Input = false,
        Output = true,
    };

    enum Level : bool
    {
        High = true,
        Low = false,
    };

    /**
     * A class to manage a single GPIO pin.
     */
    class GPIOClass
    {

    public:
        GPIOClass();

        /**
         * Similar to Arduino pinMode(pin, mode);
         * @param pinNumber
         * @param direction
         * @param pwmCapable
         */
        static void
        open(gpio_pin_t pinNumber, Direction direction, bool pwmCapable = true);

        /**
         * Defined for completeness; not actually exposed in the Arduino API
         * @param pinNumber
         */
        static void close(gpio_pin_t pinNumber);

        /**
         * Analogous to Arduino `digitalRead(pin)`.
         * @param pinNumber
         */
        static Level read(gpio_pin_t pinNumber);

        /**
         * Analogous to Arduino `analogRead(pin)`.
         */
        static uint8_t analogRead(gpio_pin_t pinNumber);

        /**
         * Analogous to Arduino `digitalWrite(pin, level)`.
         * @param pinNumber
         * @param level
         */
        static void write(gpio_pin_t pinNumber, Level level);

        /**
         * Analogous to Arduino `analogWrite(pin, level)`.
         * @param pinNumber
         * @param level
         */
        static void analogWrite(gpio_pin_t pinNumber, uint8_t level);

        virtual ~GPIOClass();
    };

} // namespace MotorHead

#ifdef __cplusplus
}
#endif

#endif // MOTOR_HEAD_UTILITY_ARDUINO_GPIO_H_