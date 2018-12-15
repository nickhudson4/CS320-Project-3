#include <iostream>
#include <string.h>
#include <cstring>
#include <sstream>
#include <chrono>

#include "aes.h"

using namespace std;

void Aes::keyExpansionCore(unsigned char* in, unsigned char i){
	LookupTables table;
	//Rotate left
	unsigned char t = in[0];
	in[0] = in[1];
	in[1] = in[2];
	in[2] = in[3];
	in[3] = t;

	//S-box four bytes
	in[0] = table.sBox[in[0]]; 
	in[1] = table.sBox[in[1]];
	in[2] = table.sBox[in[2]]; 
	in[3] = table.sBox[in[3]];

	//Rcon
	in[0] ^= table.rcon[i];


}

void Aes::keyExpansion(unsigned char* inputKey, unsigned char* expandedKeys){
	//The first 16 bytes are the original key
	for (int i = 0; i < 16; i++){
		expandedKeys[i] = inputKey[i];
	}

	int bytesGenerated = 16; //Have generated 16 so far
	int rconIteration = 1; //rcon iteration begins at 1
	unsigned char temp[4];

	while(bytesGenerated < 176){
		//Read the last 4 bytes
		for (int i = 0; i < 4; i++){
			temp[i] = expandedKeys[i + bytesGenerated - 4];
		}

		if (bytesGenerated % 16 == 0){
			keyExpansionCore(temp, rconIteration);
			rconIteration++;
		}

		for (unsigned char a = 0; a < 4; a++){
			expandedKeys[bytesGenerated] = expandedKeys[bytesGenerated - 16] ^ temp[a];
			bytesGenerated++;
		}
	}
}

void Aes::subBytes(unsigned char* state){
	LookupTables table;
	for (int i = 0; i < 16; i++){
		state[i] = table.sBox[state[i]];	
	}
}

void Aes::shiftRows(unsigned char* state){
	unsigned char tmp[16];
	tmp[0] = state[0];
	tmp[1] = state[5];
	tmp[2] = state[10];
	tmp[3] = state[15];

	tmp[4] = state[4];
	tmp[5] = state[9];
	tmp[6] = state[14];
	tmp[7] = state[3];

	tmp[8] = state[8];
	tmp[9] = state[13];
	tmp[10] = state[2];
	tmp[11] = state[7];

	tmp[12] = state[12];
	tmp[13] = state[1];
	tmp[14] = state[6];
	tmp[15] = state[11];

	for (int i = 0; i < 16; i++){
		state[i] = tmp[i];
	}
}

void Aes::mixColumns(unsigned char* state){
	
	LookupTables table;
	unsigned char tmp[16];

	tmp[0] = (unsigned char)(table.mult2[state[0]] ^ table.mult3[state[1]] ^ state[2] ^ state[3]);
	tmp[1] = (unsigned char)(state[0] ^ table.mult2[state[1]] ^ table.mult3[state[2]] ^ state[3]);
	tmp[2] = (unsigned char)(state[0] ^ state[1] ^ table.mult2[state[2]] ^ table.mult3[state[3]]);
	tmp[3] = (unsigned char)(table.mult3[state[0]] ^ state[1] ^ state[2] ^ table.mult2[state[3]]);

	tmp[4] = (unsigned char)(table.mult2[state[4]] ^ table.mult3[state[5]] ^ state[6] ^ state[7]);
	tmp[5] = (unsigned char)(state[4] ^ table.mult2[state[5]] ^ table.mult3[state[6]] ^ state[7]);
	tmp[6] = (unsigned char)(state[4] ^ state[5] ^ table.mult2[state[6]] ^ table.mult3[state[7]]);
	tmp[7] = (unsigned char)(table.mult3[state[4]] ^ state[5] ^ state[6] ^ table.mult2[state[7]]);

	tmp[8] = (unsigned char)(table.mult2[state[8]] ^ table.mult3[state[9]] ^ state[10] ^ state[11]);
	tmp[9] = (unsigned char)(state[8] ^ table.mult2[state[9]] ^ table.mult3[state[10]] ^ state[11]);
	tmp[10] = (unsigned char)(state[8] ^ state[9] ^ table.mult2[state[10]] ^ table.mult3[state[11]]);
	tmp[11] = (unsigned char)(table.mult3[state[8]] ^ state[9] ^ state[10] ^ table.mult2[state[11]]);

	tmp[12] = (unsigned char)(table.mult2[state[12]] ^ table.mult3[state[13]] ^ state[14] ^ state[15]);
	tmp[13] = (unsigned char)(state[12] ^ table.mult2[state[13]] ^ table.mult3[state[14]] ^ state[15]);
	tmp[14] = (unsigned char)(state[12] ^ state[13] ^ table.mult2[state[14]] ^ table.mult3[state[15]]);
	tmp[15] = (unsigned char)(table.mult3[state[12]] ^ state[13] ^ state[14] ^ table.mult2[state[15]]);

	for (int i = 0; i < 16; i++){
		state[i] = tmp[i];
	}

}

void Aes::addRoundKey(unsigned char* state, unsigned char* roundKey){
	for (int i = 0; i < 16; i++){
		state[i] ^= roundKey[i];
	}
}

string Aes::encryptUtil(unsigned char *key, string info){
	//All input strings must be converted to chars
	const char *message = info.c_str();  

	/* int keySize = strKey.size(); */
	/* if (keySize != 16){ //Padding key with 0s. Key must be 16 characters */
	/* 	int numZeros = 16 - keySize; */
	/* 	for (int i = 0; i < numZeros; i++){ */
	/* 		strKey += "0"; */
	/* 	} */		
	/* } */

	int originalLen = strlen((const char*)message);
	int lenOfPaddedMessage = originalLen;

	if (lenOfPaddedMessage % 16 != 0){
		lenOfPaddedMessage = (lenOfPaddedMessage / 16 + 1) * 16;
	}

	unsigned char* paddedMessage = new unsigned char[lenOfPaddedMessage];
	for (int i = 0; i < lenOfPaddedMessage; i++){
		if (i >= originalLen){
			paddedMessage[i] = 0;
		}
		else{
			paddedMessage[i] = message[i];
		}
	}

	//Now encrypt each message that is padded to be 16 bytes
	for (int i = 0; i < lenOfPaddedMessage; i+=16){
		encrypt(paddedMessage+i, key);

	}
	
	/* cout << "Encrypted message in hex: " << endl; */
	/* for (int i = 0; i < lenOfPaddedMessage; i++) { */
	/* 	std::cout << hex << (int) paddedMessage[i]; */
	/* 	std::cout << " "; */
	/* } */
	/* std::cout << std::endl; */

	string eMessage;
	for (int i = 0; i < lenOfPaddedMessage; i++){
		eMessage += paddedMessage[i];
	}

	delete [] paddedMessage;

	return eMessage;

}

void Aes::encrypt(unsigned char* message, unsigned char* key){
	unsigned char state[16];
	for (int i = 0; i < sizeof(state); i++){
		state[i] = message[i];		
	}

	int numberOfRounds = 9;
	
	unsigned char expandedKey[176];
	keyExpansion(key, expandedKey); 
	addRoundKey(state, key);

	for(int i = 0; i < numberOfRounds; i++){
		subBytes(state);
		shiftRows(state);
		mixColumns(state);
		addRoundKey(state, expandedKey + (16 * (i + 1)));
	}

	//final round that isnt looped
	subBytes(state);
	shiftRows(state);
	addRoundKey(state, expandedKey + 160);

	//copy the new encrypted message
	for (int i = 0; i < 16; i++){
		message[i] = state[i];
	}

}

void Aes::subRoundKey(unsigned char * state, unsigned char * roundKey) {
	for (int i = 0; i < 16; i++) {
		state[i] ^= roundKey[i];
	}
}

void Aes::inverseMixColumns(unsigned char * state) {
	LookupTables table;
	unsigned char tmp[16];

	tmp[0] = (unsigned char)table.mult14[state[0]] ^ table.mult11[state[1]] ^ table.mult13[state[2]] ^ table.mult9[state[3]];
	tmp[1] = (unsigned char)table.mult9[state[0]] ^ table.mult14[state[1]] ^ table.mult11[state[2]] ^ table.mult13[state[3]];
	tmp[2] = (unsigned char)table.mult13[state[0]] ^ table.mult9[state[1]] ^ table.mult14[state[2]] ^ table.mult11[state[3]];
	tmp[3] = (unsigned char)table.mult11[state[0]] ^ table.mult13[state[1]] ^ table.mult9[state[2]] ^ table.mult14[state[3]];

	tmp[4] = (unsigned char)table.mult14[state[4]] ^ table.mult11[state[5]] ^ table.mult13[state[6]] ^ table.mult9[state[7]];
	tmp[5] = (unsigned char)table.mult9[state[4]] ^ table.mult14[state[5]] ^ table.mult11[state[6]] ^ table.mult13[state[7]];
	tmp[6] = (unsigned char)table.mult13[state[4]] ^ table.mult9[state[5]] ^ table.mult14[state[6]] ^ table.mult11[state[7]];
	tmp[7] = (unsigned char)table.mult11[state[4]] ^ table.mult13[state[5]] ^ table.mult9[state[6]] ^ table.mult14[state[7]];

	tmp[8] = (unsigned char)table.mult14[state[8]] ^ table.mult11[state[9]] ^ table.mult13[state[10]] ^ table.mult9[state[11]];
	tmp[9] = (unsigned char)table.mult9[state[8]] ^ table.mult14[state[9]] ^ table.mult11[state[10]] ^ table.mult13[state[11]];
	tmp[10] = (unsigned char)table.mult13[state[8]] ^ table.mult9[state[9]] ^ table.mult14[state[10]] ^ table.mult11[state[11]];
	tmp[11] = (unsigned char)table.mult11[state[8]] ^ table.mult13[state[9]] ^ table.mult9[state[10]] ^ table.mult14[state[11]];

	tmp[12] = (unsigned char)table.mult14[state[12]] ^ table.mult11[state[13]] ^ table.mult13[state[14]] ^ table.mult9[state[15]];
	tmp[13] = (unsigned char)table.mult9[state[12]] ^ table.mult14[state[13]] ^ table.mult11[state[14]] ^ table.mult13[state[15]];
	tmp[14] = (unsigned char)table.mult13[state[12]] ^ table.mult9[state[13]] ^ table.mult14[state[14]] ^ table.mult11[state[15]];
	tmp[15] = (unsigned char)table.mult11[state[12]] ^ table.mult13[state[13]] ^ table.mult9[state[14]] ^ table.mult14[state[15]];

	for (int i = 0; i < 16; i++) {
		state[i] = tmp[i];
	}
}

// Shifts rows right (rather than left) for decryption
void Aes::dShiftRows(unsigned char * state) {
	unsigned char tmp[16];

	/* Column 1 */
	tmp[0] = state[0];
	tmp[1] = state[13];
	tmp[2] = state[10];
	tmp[3] = state[7];

	/* Column 2 */
	tmp[4] = state[4];
	tmp[5] = state[1];
	tmp[6] = state[14];
	tmp[7] = state[11];

	/* Column 3 */
	tmp[8] = state[8];
	tmp[9] = state[5];
	tmp[10] = state[2];
	tmp[11] = state[15];

	/* Column 4 */
	tmp[12] = state[12];
	tmp[13] = state[9];
	tmp[14] = state[6];
	tmp[15] = state[3];

	for (int i = 0; i < 16; i++) {
		state[i] = tmp[i];
	}
}

void Aes::dSubBytes(unsigned char * state) {
	LookupTables table;
	for (int i = 0; i < 16; i++) { // Perform substitution to each of the 16 bytes
		state[i] = table.inv_s[state[i]];
	}
}

void Aes::round(unsigned char * state, unsigned char * key) {
	subRoundKey(state, key);
	inverseMixColumns(state);
	dShiftRows(state);
	dSubBytes(state);
}

// Same as Round() but no InverseMixColumns
void Aes::initialRound(unsigned char * state, unsigned char * key) {
	subRoundKey(state, key);
	dShiftRows(state);
	dSubBytes(state);
}

string Aes::decryptUtil(unsigned char *key, string msgstr){
	char * msg = new char[msgstr.size()+1];

	strcpy(msg, msgstr.c_str());

	int n = strlen((const char*)msg);

	unsigned char * encryptedMessage = new unsigned char[n];
	for (int i = 0; i < n; i++) {
		encryptedMessage[i] = (unsigned char)msg[i];
	}	

	int messageLength = strlen((const char*)encryptedMessage);
	unsigned char* decryptedMessage = new unsigned char[messageLength]; 

	for (int i = 0; i < messageLength; i += 16){
		decrypt(encryptedMessage + i, key, decryptedMessage + i);
	}

	/* std::cout << "Decrypted Message:" << std::endl; */
	/* for (int i = 0; i < messageLength; i++){ */
	/* 	std::cout << decryptedMessage[i]; */
	/* } */
	/* std::cout << std::endl; */
	string dMessage;
	for (int i = 0; i < messageLength; i++){
		dMessage += decryptedMessage[i];
	}

	delete [] msg;
	delete [] encryptedMessage;
	delete [] decryptedMessage;

	return dMessage;
}

void Aes::decrypt(unsigned char* encryptedMessage, unsigned char* key, unsigned char* decryptedMessage){
	unsigned char state[16];

	for (int i = 0; i < 16; i++){
		state[i] = encryptedMessage[i];
	}
	unsigned char expandedKey[176];
	keyExpansion(key, expandedKey);

	initialRound(state, expandedKey+160);

	int numberOfRounds = 9;

	for (int i = 8; i >= 0; i--){
		round(state, expandedKey + (16 * (i + 1)));
	}

	subRoundKey(state, expandedKey);

	for (int i = 0; i < 16; i++){
		decryptedMessage[i] = state[i];
	}
}

//Below are Misc methods

void Aes::timedEncryptNumWords(int n){ //Tests speed on number of words n encrypted
	std::cout << "==============================" << std::endl;

	float eTime = 0;
	float dTime = 0;
	unsigned char testKey[16] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	for (int i = 0; i < n; i++){
		/* auto start = std::chrono::system_clock::now(); */
		/* string e = encryptUtil(testKey, to_string(i)); */		
		/* auto end = std::chrono::system_clock::now(); */
		/* std::chrono::duration<double> elapsed_seconds = end-start; */
		/* std::time_t end_time = std::chrono::system_clock::to_time_t(end); */
		/* eTime += elapsed_seconds.count(); */

		/* auto start2 = std::chrono::system_clock::now(); */
		/* string d = decryptUtil(testKey, e); */ 
		/* auto end2 = std::chrono::system_clock::now(); */
		/* std::chrono::duration<double> elapsed_seconds2 = end2-start2; */
		/* std::time_t end_time2 = std::chrono::system_clock::to_time_t(end2); */
		/* dTime += elapsed_seconds2.count(); */

		auto start = std::chrono::system_clock::now();
		string e = encryptUtil(testKey, to_string(i));		
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end-start;
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		eTime += elapsed_seconds.count();

		auto start2 = std::chrono::system_clock::now();
		string d = decryptUtil(testKey, e); 
		auto end2 = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds2 = end2-start2;
		std::time_t end_time2 = std::chrono::system_clock::to_time_t(end2);
		dTime += elapsed_seconds2.count();
	}

	std::cout << "Time to Encrypt " << n << " Words: " << eTime << " seconds" << std::endl; 
	std::cout << "Time to Decrypt " << n << " Words: " << dTime << " seconds" << std::endl; 

}

void Aes::timedEncryptSizeWords(int n){ //Tests sppeed on size word n encrypted
	std::cout << "==============================" << std::endl;
	std::cout << "**Timing Encrypt/Decrypt Based on Word Size**" << std::endl;

	float eTime = 0;
	float dTime = 0;
	unsigned char testKey[16] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	string word;
	
	for (int i = 0; i < n; i++){ //Make word of length n
		word += "a";
	}
	auto start = std::chrono::system_clock::now();
	string e = encryptUtil(testKey, word);		
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	eTime += elapsed_seconds.count();

	auto start2 = std::chrono::system_clock::now();
	string d = decryptUtil(testKey, e); 
	auto end2 = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds2 = end2-start2;
	std::time_t end_time2 = std::chrono::system_clock::to_time_t(end2);
	dTime += elapsed_seconds2.count();

	std::cout << "Time to Encrypt " << n << " Sized Word: " << eTime << " seconds" << std::endl; 
	std::cout << "Time to Decrypt " << n << " Sized Word: " << dTime << " seconds" << std::endl; 
}
