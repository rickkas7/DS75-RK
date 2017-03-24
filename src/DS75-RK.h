#ifndef __DS75_RK_H
#define __DS75_RK_H


class DS75 {
public:
	/**
	 * You typically create one of these objects on the stack.
	 *
	 * The first argument is typically Wire (pins D0 and D1). On the Electron it can also be Wire1 (pins C4 and C5)
	 *
	 * The addr argument is the address 0-7 based on the setting of A0, A1 and A2.
	 */
	DS75(TwoWire &wire, int addr = 0);
	virtual ~DS75();

	/**
	 * Typically called during setup() to start the Wire interface.
	 */
	void begin();

	/**
	 * Gets the current temperature (in degrees Celsius)
	 *
	 * The sensor constantly samples the temperature and this function returns the
	 * most recent reading. See setResolution for how often it's updated.
	 */
	float getTemperature();

	/**
	 * Set the TOS temperature level (in degrees Celsius)
	 *
	 * This is the higher limit, which signals the output when this level is exceeded.
	 */
	bool setTOS(float temp);

	/**
	 * Sets the THYST temperature level (in degrees Celsius)
	 *
	 * This is the lower level. When the temperature falls below this level, the high temperature
	 * alarm is reset.
	 */
	bool setTHYST(float temp);

	/**
	 * Converts temperature in Fahrenheit to Celsius.
	 */
	static float convertFtoC(float temp);

	/**
	 * Converts temperature in Celsius to Fahrenheit.
	 *
	 * Note that all of the functions like getTemperature(), setTOS() and setTHYST() all take temperatures
	 * in Celsius.
	 */
	static float convertCtoF(float temp);

	/**
	 * Set the resolution of the temperature 9 <= numBits <= 12
	 *
	 * bits  time per sample  resolution degrees C
	 *  9     150 ms          0.5    (1/2)
	 * 10     300 ms          0.24   (1/4)
	 * 11     600 ms          0.125  (1/8)
	 * 12    1200 ms          0.0625 (1/16)
	 */
	bool setResolution(uint8_t numBits);

	/**
	 * 0 <= value <= 3
	 *
	 * value  number of samples above TOS required
	 * 0      1
	 * 1      2
	 * 2      4
	 * 3      6
	 */
	bool setFaultTolerance(uint8_t value);

	/**
	 * Set the polarity of the OS output.
	 *
	 * The OS output is open collector.
	 *
	 * pol = false = active low (default)
	 * pol = true = active high
	 */
	bool setOSPolarity(bool pol);

	/**
	 * Sets interrupt mode (true) or comparator mode (false)
	 *
	 * Default: false
	 */
	bool setInterruptMode(bool interruptMode);

	/**
	 * Set to true to enter shutdown mode, with sampling temperature turned off to
	 * save power. The I2C interface remains active so you can turn it back on again.
	 */
	bool setShutdown(bool shutdown);

	uint8_t readRegister(uint8_t reg);
	bool writeRegister(uint8_t reg, uint8_t value);

	uint16_t readRegister16(uint16_t reg);
	bool writeRegister16(uint16_t reg, uint16_t value);


	/**
	 * Used internally to convert the weird 2's complement temperature values to a float
	 */
	float tempToFloat(uint16_t temp);

	/**
	 * Used internally to a float to the weird 2's complement temperature values
	 */
	uint16_t floatToTemp(float f);

	static const uint8_t DEVICE_ADDR = 0b1001000;

	static const uint8_t REG_TEMP = 0b00;
	static const uint8_t REG_CONFIG = 0b01;
	static const uint8_t REG_THYST = 0b10;
	static const uint8_t REG_TOS = 0b11;



protected:
	TwoWire &wire;
	int addr; // This is just 0-7, the (0b0100000 of the 7-bit address is ORed in later)

};

#endif /* __DS75_RK_H */
