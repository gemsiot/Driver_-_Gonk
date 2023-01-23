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

    // Wire.beginTransmission(ADR);
	// Wire.write(0x09); //Write to voltage register
	// int Error = Wire.endTransmission();

    // Wire.beginTransmission(ADR);
    // Wire.write(0x40); //Write to led register //DEBUG!
    // Wire.write(0b11100100); //Set for always on, 4 bars
    // // Wire.write(0b10100100); //Set for push button control, 4 bars
    // Wire.write(0b01110010); //Default, but breathing LEDs
    // Wire.endTransmission();
	return ""; //DEBUG!
}

// String Gonk::getData(time_t time)
// {
// 	// String output = "{\"GONK\":"; //OPEN JSON BLOB
// 	// if(isPresent()) { //If Talon has been detected, go through normal data appending 
// 	// 	output = output + "{"; //Open sub-blob
// 	// 	output = output + "\"CellV\":" + String(getBatteryData(0x09)*0.078125, 6) + ","; //Convert to volts
// 	// 	output = output + "\"CellVAvg\":" + String(getBatteryData(0x19)*0.078125, 6) + ","; //Convert to volts
// 	// 	output = output + "\"CapLeft\":" + String(getBatteryData(0x05)*0.5, 1) + ","; //Convert to mAh
// 	// 	output = output + "\"CapTotal\":" + String(getBatteryData(0x10)*0.5, 1) + ","; //Convert to mAh
// 	// 	output = output + "\"TTF\":" + String(getBatteryData(0x20)*5.625, 3) + ","; //Convert to seconds
// 	// 	output = output + "\"SoC\":" + String(getBatteryData(0x06)/256.0, 2); //Convert to %
// 	// 	output = output + "}}";
// 	// }
// 	// else output = output + "null}"; //Terminate output with null
// 	// return output;
// 	return "\"Gonk\":null"; //Return null since no actual data
// }

// String Gonk::getErrors()
// {
//     return "{}"; //DEBUG!
// }

// String Gonk::getMetadata()
// {
//     return "{}"; //DEBUG!
// }

String Gonk::selfDiagnostic(uint8_t diagnosticLevel, time_t time)
{
	
    // String output = "{\"Apogee Pyro\":{";
	String output = "\"GONK\":{"; //OPEN JSON BLOB
	if(isPresent()) { //If Talon has been detected, go through normal diagnostic return appending 
		if(diagnosticLevel == 0) {
			//TBD
			// output = output + "\"lvl-0\":{},";
			// return output + "\"lvl-0\":{},\"Pos\":[" + String(port) + "]}}";
		}

		if(diagnosticLevel <= 1) {
			//TBD
			// output = output + "\"lvl-1\":{},";
		}

		if(diagnosticLevel <= 2) {
			//TBD
			// output = output + "\"lvl-2\":{},";
		}

		if(diagnosticLevel <= 3) {
			//TBD
			// Serial.println(millis()); //DEBUG!
			// output = output + "\"lvl-3\":{"; //OPEN JSON BLOB

			// output = output + "},"; //CLOSE JSON BLOB
			// return output + ",\"Pos\":[" + String(port) + "]}}";
			// return output;

		}

		if(diagnosticLevel <= 4) {
			// String output = selfDiagnostic(5); //Call the lower level of self diagnostic 
			// output = output.substring(0,output.length() - 1); //Trim off closing brace
			// output = output + "\"lvl-4\":{"; //OPEN JSON BLOB
			// uint8_t adr = (talon.sendCommand("?!")).toInt(); //Get address of local device 
			// String stat = talon.command("M2", adr);
			// Serial.print("STAT: "); //DEBUG!
			// Serial.println(stat);

			// delay(1000); //Wait 1 second to get data back //FIX! Wait for newline??
			// String data = talon.command("D0", adr);
			// Serial.print("DATA: "); //DEBUG!
			// Serial.println(data);
			// data.remove(0,2); //Trim leading address and +
			// float angle = (data.trim()).toFloat();
			// output = output + "\"Angle\":" + String(angle);
			// output = output + "},"; //CLOSE JSON BLOB
			// return output + ",\"Pos\":[" + String(port) + "]}}";
			// return output;

		}

		if(diagnosticLevel <= 5) {
			// output = output + "\"lvl-5\":{"; //OPEN JSON BLOB
			// output = output + "{"; //Open sub-blob
			output = output + "\"CellV\":" + String(getBatteryData(0x09)*0.078125, 6) + ","; //Convert to volts
			output = output + "\"CellVAvg\":" + String(getBatteryData(0x19)*0.078125, 6) + ","; //Convert to volts
			output = output + "\"CapLeft\":" + String(getBatteryData(0x05)*0.5, 1) + ","; //Convert to mAh
			output = output + "\"CapTotal\":" + String(getBatteryData(0x10)*0.5, 1) + ","; //Convert to mAh
			output = output + "\"TTF\":" + String(getBatteryData(0x20)*5.625, 3) + ","; //Convert to seconds
			output = output + "\"SoC\":" + String(getBatteryData(0x06)/256.0, 2) + ","; //Convert to %
			output = output + "\"Temperature\":" + String(getBatteryData(0x34)/256.0, 2) + ","; //Convert to C°
			// output = output + "}}";
			// output = output + "}"; //Close pair
			
		}
		return output + "\"Pos\":[" + getTalonPortString() + "," + getSensorPortString() + "]}"; //Write position in logical form - Return compleated closed output

		
			
	}
	// else output = output + "null}"; //Terminate output with null
	else output = "\"GONK\":null"; //Terminate output with null
	return output; 
}
String Gonk::getMetadata()
{
	String metadata = "\"Gonk\":{";

    String batUUID = getUUIDString(); 
    if(!batUUID.equals("null")) batUUID = "\"" + batUUID + "\""; //If not null, wrap with quotes for JSON, otherwise leave as null 
    metadata = metadata + "\"RTC UUID\":" + batUUID + ","; //Append RTC UUID
	metadata = metadata + "\"Firmware\":\"v" + FIRMWARE_VERSION + "\","; //Report firmware version as modded BCD
	metadata = metadata + "\"Pos\":[" + getTalonPortString() + "]"; //Concatonate position 
	metadata = metadata + "}"; //CLOSE  
	return metadata;
}
String Gonk::getUUIDString()
{
	Wire.beginTransmission(ADR);
	Wire.write(0xBB); //Setup to read from Config reg
	int error = Wire.endTransmission();

	uint8_t configTemp = 0;
	uint8_t val = 0; 
	String uuid = "";
	if(error == 0) {
		Wire.requestFrom(ADR, 1);
		configTemp = Wire.read();
		Wire.beginTransmission(ADR);
		Wire.write(0xBB);
		Wire.write(configTemp & 0xCF); //Clear AtRateEN and DPEn bits
		Wire.endTransmission();

		Wire.beginTransmission(ADR);
		Wire.write(0xD4); //Point to begining of SN region
		Wire.endTransmission();

		Wire.requestFrom(ADR, 8); 
		for(int i = 0; i < 8; i++) {
			val = Wire.read();
			uuid = String(val, HEX) + uuid; //Concatonate into full UUID (IN REVERSE ORDER)
			if(i < 7) uuid = "-" + uuid; //Print formatting chracter, don't print on last pass
		}
		Wire.beginTransmission(ADR);
		Wire.write(0xBB);
		Wire.write(configTemp); //Write origional config2 back
		Wire.endTransmission();

		return uuid; //Return assembled uuid
	}
	else return "null"; //Return null if there is a read error 
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
	String output = "\"GONK\":{"; // OPEN JSON BLOB
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
	output = output + "\"Pos\":[" + getTalonPortString() + "]"; //Concatonate position 
	output = output + "}"; //CLOSE JSON BLOB
	numErrors = 0; //Clear error count
	return output;

	// return -1; //Return fault if unknown cause 
}

bool Gonk::setIndicatorState(uint8_t mode)
{
	Wire.beginTransmission(ADR);
	// Wire.write(0x09); //Write to voltage register
	int error = Wire.endTransmission();
	if(mode == GonkIndicatorMode::SOLID) { //If true, turn on manually
		Wire.beginTransmission(ADR);
		Wire.write(0x40); //Write to led register //DEBUG!
		Wire.write(0b11100100); //Set for always on, 4 bars
		// Wire.write(0b10100100); //Set for push button control, 4 bars
		// Wire.write(0b01110010); //Default, but breathing LEDs
		Wire.write(0b01110000); //Default, solid LEDs
		Wire.endTransmission();
	}
	else if(mode == GonkIndicatorMode::FULLY_OFF) { //Otherwise relinquish to push button control
		Wire.beginTransmission(ADR);
		Wire.write(0x40); //Write to led register //DEBUG!
		// Wire.write(0b11100100); //Set for always on, 4 bars
		Wire.write(0b00100100); //Disable LEDs, 4 bars
		Wire.write(0b01110000); //Default, solid LEDs
		Wire.endTransmission();
	}
	else if(mode == GonkIndicatorMode::PUSH_BUTTON) { //Otherwise relinquish to push button control
		Wire.beginTransmission(ADR);
		Wire.write(0x40); //Write to led register //DEBUG!
		// Wire.write(0b11100100); //Set for always on, 4 bars
		Wire.write(0b10000100); //Set for push button control, 4 bars
		// Wire.write(0b00100100); //Disable LEDs, 4 bars
		Wire.write(0b01110001); //Default, fill animation
		Wire.endTransmission();
	}
	else if(mode == GonkIndicatorMode::BLINKING) { //Otherwise relinquish to push button control
		Wire.beginTransmission(ADR);
		Wire.write(0x40); //Write to led register //DEBUG!
		Wire.write(0b11100100); //Set for always on, 4 bars
		// Wire.write(0b10100100); //Set for push button control, 4 bars
		// Wire.write(0b00100100); //Disable LEDs, 4 bars
		// Wire.write(0b01110000); //Default, solid LEDs
		Wire.write(0b01111110); //Default, but breathing LEDs
		Wire.endTransmission();
	}
	if(error == 0) return true;
	else {
		Serial.print("Error on Bat Com - "); //DEBUG!
		Serial.println(error); //DEBUG!
		//THROW ERROR
		return false;
	}
}

bool Gonk::isPresent()
{
	Wire.beginTransmission(ADR);
	// Wire.write(0x09); //Write to voltage register
	int error = Wire.endTransmission();
	if(error == 0) return true;
	else return false; 
}

