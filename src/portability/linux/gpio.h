#ifndef MOTOR_HEAD_UTILITY_LINUX_GPIO_H_
#define MOTOR_HEAD_UTILITY_LINUX_GPIO_H_
#ifndef ARDUINO
    #include <cstdint>
    #include <stdexcept>

    #ifdef __cplusplus
extern "C" {
    #endif

namespace MotorHead {
    typedef uint8_t gpio_pin_t;
    const gpio_pin_t NULL_PIN = 0xFF;

    #ifndef MOTOR_HEAD_LINUX_GPIO_CHIP
        /**
         * The default GPIO chip to use.  Defaults to `/dev/gpiochip4` (for RPi5).
         * Falls back to `/dev/gpiochip0` if this value is somehow incorrect.
         */
        #define MOTOR_HEAD_LINUX_GPIO_CHIP "/dev/gpiochip4"
    #endif

    /** Specific exception for GPIO errors */
    class GPIOException : public std::runtime_error
    {
    public:
        explicit GPIOException(const std::string& msg) : std::runtime_error(msg) {}
    };

    /// A struct to manage the GPIO chip file descriptor.
    /// This struct's destructor should close any cached GPIO pin requests' file
    /// descriptors.
    struct GPIOChipCache
    {
        const char* chip = MOTOR_HEAD_LINUX_GPIO_CHIP;
        int fd = -1;
        bool chipInitialized = false;

        /// Open the File Descriptor for the GPIO chip
        void openDevice();

        /// Close the File Descriptor for the GPIO chip
        void closeDevice();

        /// should be called automatically on program start.
        /// Here, we do some one-off configuration.
        GPIOChipCache();

        /// Should be called automatically on program exit.
        /// What we need here is to make sure that the File Descriptors used to
        /// control GPIO pins are properly closed.
        ~GPIOChipCache();
    };

    enum Direction
    {
        Input = false,
        Output = true,
    };

    enum Level
    {
        High = true,
        Low = false,
    };
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
         * @param port
         */
        static void close(gpio_pin_t pinNumber);

        /**
         * Similar to Arduino digitalRead(pin);
         * @param port
         */
        static Level read(gpio_pin_t pinNumber);

        /**
         * Similar to Arduino digitalWrite(pin, level);
         * @param pinNumber
         * @param value
         */
        static void write(gpio_pin_t pinNumber, Level value);

        virtual ~GPIOClass();
    };

} // namespace MotorHead

    #ifdef __cplusplus
}
    #endif

#endif // ARDUINO
#endif // MOTOR_HEAD_UTILITY_LINUX_GPIO_H_