
#include "Particle.h"
#include "DS75-RK.h"


DS75::DS75(TwoWire &wire, int addr) :
	wire(wire), addr(addr) {
}

DS75::~DS75() {
}

void DS75::begin() {
	wire.begin();
}

float DS75::getTemperature() {
	uint16_t temp = readRegister16(REG_TEMP);

	return tempToFloat(temp);
}

bool DS75::setTOS(float temp) {
	return writeRegister16(REG_TOS, floatToTemp(temp));
}

bool DS75::setTHYST(float temp) {
	return writeRegister16(REG_THYST, floatToTemp(temp));
}


float DS75::tempToFloat(uint16_t temp) {
	int8_t wholePart = (int8_t) (temp >> 8);
	uint16_t fracPart = (temp >> 4) & 0xf;

	// Serial.printlnf("temp=%x wholePart=%d fracPart=%d", temp, wholePart, fracPart);

	return (float)wholePart + ((float)fracPart * 0.0625);
}

uint16_t DS75::floatToTemp(float f) {
	uint8_t wholePart = (int8_t)f;

	uint16_t fracPart = ((uint16_t) (f * 16)) % 16;

	return (((uint16_t)wholePart) << 8) | (fracPart << 4);
}

float DS75::convertFtoC(float temp) {
	return (temp - 32.0) * 5.0 / 9.0;
}
float DS75::convertCtoF(float temp) {
	return (temp * 9.0) / 5.0 + 32.0;
}


bool DS75::setResolution(uint8_t numBits) {
	if (numBits < 9 || numBits > 12) {
		return false;
	}

	uint8_t regValue = readRegister(REG_CONFIG);

	regValue &= 0b00011111;
	regValue |= (numBits - 9) << 5;

	return writeRegister(REG_CONFIG, regValue);
}

bool DS75::setFaultTolerance(uint8_t value) {
	if (value > 3) {
		return false;
	}
	uint8_t regValue = readRegister(REG_CONFIG);

	regValue &= 0b01100111;
	regValue |= value << 3;

	return writeRegister(REG_CONFIG, regValue);
}

bool DS75::setOSPolarity(bool pol) {
	uint8_t regValue = readRegister(REG_CONFIG);

	regValue &= 0b01111011;
	if (pol) {
		regValue |= 0b100;
	}

	return writeRegister(REG_CONFIG, regValue);
}


bool DS75::setInterruptMode(bool interruptMode) {
	uint8_t regValue = readRegister(REG_CONFIG);

	regValue &= 0b01111101;
	if (interruptMode) {
		regValue |= 0b10;
	}

	return writeRegister(REG_CONFIG, regValue);
}

bool DS75::setShutdown(bool shutdown) {
	uint8_t regValue = readRegister(REG_CONFIG);

	regValue &= 0b01111110;
	if (shutdown) {
		regValue |= 0b1;
	}

	return writeRegister(REG_CONFIG, regValue);
}


uint8_t DS75::readRegister(uint8_t reg) {
	wire.beginTransmission(addr | DEVICE_ADDR);
	wire.write(reg);
	wire.endTransmission(false);

	wire.requestFrom(addr | DEVICE_ADDR, 1, true);
	uint8_t value = (uint8_t) wire.read();

	// Serial.printlnf("readRegister reg=%d value=%d", reg, value);

	return value;
}

bool DS75::writeRegister(uint8_t reg, uint8_t value) {
	wire.beginTransmission(addr | DEVICE_ADDR);
	wire.write(reg);
	wire.write(value);

	int stat = wire.endTransmission(true);

	// Serial.printlnf("writeRegister reg=%d value=%d stat=%d read=%d", reg, value, stat, readRegister(reg));

	return (stat == 0);
}

uint16_t DS75::readRegister16(uint16_t reg) {
	wire.beginTransmission(addr | DEVICE_ADDR);
	wire.write(reg);
	wire.endTransmission(false);

	wire.requestFrom(addr | DEVICE_ADDR, 2, true);
	uint16_t value = ((uint16_t) wire.read() << 8);
	value |= (uint16_t) wire.read();

	return value;
}

bool DS75::writeRegister16(uint16_t reg, uint16_t value) {
	wire.beginTransmission(addr | DEVICE_ADDR);
	wire.write(reg);
	wire.write(value >> 8);
	wire.write(value);

	int stat = wire.endTransmission(true);
	return (stat == 0);
}
