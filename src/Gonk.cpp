#include <Gonk.h>

Gonk::Gonk(uint8_t talonPort_, uint8_t sensorPort_, uint8_t version)
{
	talonPort = talonPort_ - 1;
	// sensorPort = sensorPort_ - 1;
	sensorInterface = BusType::NONE; 
}

String Gonk::begin(time_t time, bool &criticalFault, bool &fault)
{
    //Only use isEnabled() if using particle
	#if defined(ARDUINO) && ARDUINO >= 100 
		Wire.begin();
	#elif defined(PARTICLE)
		if(!Wire.isEnabled()) Wire.begin(); //Only initialize I2C if not done already //INCLUDE FOR USE WITH PARTICLE 
	#endif

    Wire.beginTransmission(ADR);
	Wire.write(0x09); //Write to voltage register
	int Error = Wire.endTransmission();

    Wire.beginTransmission(ADR);
    Wire.write(0x40); //Write to led register //DEBUG!
    // Wire.write(0b11100100); //Set for always on, 4 bars
    Wire.write(0b10100100); //Set for push button control, 4 bars
    Wire.write(0b01110010); //Default, but breathing LEDs
    Wire.endTransmission();
	return "{}"; //DEBUG!
}

String Gonk::getData(time_t time)
{
	String output = "{\"GONK\":"; //OPEN JSON BLOB
	if(isPresent()) { //If Talon has been detected, go through normal data appending 
		output = output + "{"; //Open sub-blob
		output = output + "\"CellV\":" + String(getBatteryData(0x09)*0.078125, 6) + ","; //Convert to volts
		output = output + "\"CellVAvg\":" + String(getBatteryData(0x19)*0.078125, 6) + ","; //Convert to volts
		output = output + "\"CapLeft\":" + String(getBatteryData(0x05)*0.5, 1) + ","; //Convert to mAh
		output = output + "\"CapTotal\":" + String(getBatteryData(0x10)*0.5, 1) + ","; //Convert to mAh
		output = output + "\"TTF\":" + String(getBatteryData(0x20)*5.625, 3) + ","; //Convert to seconds
		output = output + "\"SoC\":" + String(getBatteryData(0x06)/256.0, 2); //Convert to %
		output = output + "}}";
	}
	else output = output + "null}"; //Terminate output with null
	return output; 
}

// String Gonk::getErrors()
// {
//     return "{}"; //DEBUG!
// }

String Gonk::getMetadata()
{
    return "{}"; //DEBUG!
}

String Gonk::selfDiagnostic(uint8_t diagnosticLevel, time_t time)
{
    return "{}";
}

uint16_t Gonk::getBatteryData(uint8_t Reg)
{
	// const uint8_t ADR = 0x36;

	Wire.beginTransmission(ADR);
	Wire.write(Reg); //Write to voltage register
	int Error = Wire.endTransmission();
	delay(2);
	int Data = 0; 
	// Wire.requestFrom(ADR, 2);
	Wire.requestFrom(ADR, 2);
	Data = Wire.read();
	Data = Data | (Wire.read() << 8);

	return Data;
}

// int Gonk::throwError(uint32_t error)
// {
// 	errors[(numErrors++) % MAX_NUM_ERRORS] = error; //Write error to the specified location in the error array
// 	if(numErrors > MAX_NUM_ERRORS) errorOverwrite = true; //Set flag if looping over previous errors 
// 	return numErrors;
// }

String Gonk::getErrors()
{
	String output = "{\"GONK\":{"; // OPEN JSON BLOB
	output = output + "\"CODES\":["; //Open codes pair

	for(int i = 0; i < min(MAX_NUM_ERRORS, numErrors); i++) { //Interate over used element of array without exceeding bounds
		output = output + "\"0x" + String(errors[i], HEX) + "\","; //Add each error code
		errors[i] = 0; //Clear errors as they are read
	}
	if(output.substring(output.length() - 1).equals(",")) {
		output = output.substring(0, output.length() - 1); //Trim trailing ','
	}
	output = output + "],"; //close codes pair
	output =  output + "\"OW\":"; //Open state pair
	if(numErrors > MAX_NUM_ERRORS) output = output + "1,"; //If overwritten, indicate the overwrite is true
	else output = output + "0,"; //Otherwise set it as clear
	output = output + "\"NUM\":" + String(numErrors) + ","; //Append number of errors
	output = output + "\"Pos\":[" + String(talonPort + 1) + "]"; //Concatonate position 
	output = output + "}}"; //CLOSE JSON BLOB
	numErrors = 0; //Clear error count
	return output;

	// return -1; //Return fault if unknown cause 
}

bool Gonk::isPresent()
{
	Wire.beginTransmission(ADR);
	// Wire.write(0x09); //Write to voltage register
	int error = Wire.endTransmission();
	if(error == 0) return true;
	else return false; 
}

