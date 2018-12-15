#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <chrono>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <cstring>

#include "hashTable.h"
#include "aes.h"

using namespace std;

Dictionary::Dictionary(){
	for (int i = 0; i < tableSize; i++){
		hTable[i] = new hashNode;
		hTable[i]->userName = "emptyBucket";
		hTable[i]->name = "emptyBucket";
		hTable[i]->phoneNumber = "emptyBucket";
		hTable[i]->password = "emptyBucket";

		hTable[i]->next = nullptr;
	}
}

void Dictionary::printTable(){
	hashNode *tmp;
	for (int i = 0; i < tableSize; i++){
		tmp = hTable[i];
		std::cout << "[" << i << "]" << tmp->userName;
		while(tmp->next != nullptr){
			std::cout << "--->" <<  tmp->userName;
			tmp = tmp->next;
		}
		std::cout << std::endl;
	}
}

bool Dictionary::login(string userName, unsigned char *password){
	Aes aes;
	bool check = userSearch(userName);
	string keyString;
	for (int i = 0; i < sizeof(password); i++){
		keyString += password[i];
	}
	if (check == true){ //If user exists, check if encrypted passwords match in the bucket
		string encryptedPassword = aes.encryptUtil(password, keyString); 
		hashNode *tmp;
		long key = hash(userName);
		tmp = hTable[key];
		if (tmp->userName == userName){
			if (encryptedPassword == tmp->password){
				string clearName = aes.decryptUtil(password, tmp->name);
				string clearPhone = aes.decryptUtil(password, tmp->phoneNumber);
				std::cout << "USER FOUND: " << std::endl;
				printUserUtil(tmp->userName, clearName, clearPhone);
				return true;
			}
		}
		else{
			while(tmp->next != nullptr){
				if (tmp->userName == userName){
					if (encryptedPassword == tmp->password){
						string clearName = aes.decryptUtil(password, tmp->name);
						string clearPhone = aes.decryptUtil(password, tmp->phoneNumber);

						std::cout << "USER FOUND: " << std::endl;
						printUserUtil(tmp->userName, clearName, clearPhone);
						return true;
					}
				}
				tmp = tmp->next;
				if (tmp->userName == userName){
					if (encryptedPassword == tmp->password){
						string clearName = aes.decryptUtil(password, tmp->name);
						string clearPhone = aes.decryptUtil(password, tmp->phoneNumber);
						printUserUtil(tmp->userName, clearName, clearPhone);
						std::cout << "USER FOUND:" << std::endl;
						return true;
					}
				}
			}
		}
		
	}
	else{
		std::cout << "**Incorrect Username!**" << std::endl;
		return false;
	}
	std::cout << "**Incorrect Password for "<< userName << "!**" << std::endl;
	return false;

}

bool Dictionary::userSearch(string userName){
	hashNode *tmp;
	long key = hash(userName);
	tmp = hTable[key];
	if (tmp->userName == userName){
		return true;
	}
	else{
		while(tmp->next != nullptr){
			if (tmp->userName == userName){
				return true;
			}
			tmp = tmp->next;
			if (tmp->userName == userName){
				return true;
			}
		}
	}
	return false;
}

void Dictionary::addUser(string userName, string name, string phoneNumber, unsigned char *password){
	Aes aes;
	long index = hash(userName);
	string keyString;
	for (int i = 0; i < sizeof(password); i++){ //Putting password(key) into string to encrypt it
		keyString += password[i];	
	}
	string encryptedName = aes.encryptUtil(password, name); 
	string encryptedPhone = aes.encryptUtil(password,phoneNumber);
	string encryptedPassword = aes.encryptUtil(password, keyString);
	

	if (hTable[index]->userName == "emptyBucket"){
		hTable[index]->userName = userName;
		hTable[index]->name = encryptedName;
		hTable[index]->phoneNumber = encryptedPhone;
		hTable[index]->password = encryptedPassword;
		hTable[index]->key = index;
	}
	else {
		hashNode *tmp = hTable[index];
		hashNode *newNode;
		newNode->userName = userName; //Stores the word for node
		newNode->name = encryptedName;
		newNode->phoneNumber = encryptedPhone;
		newNode->password = encryptedPassword;
		newNode->key = index; //Stores the key for node
		newNode->next = nullptr;
		while(tmp->next != nullptr){
			tmp = tmp->next;
		}

		tmp->next = newNode;
	}
}

void Dictionary::printUserUtil(string username, string name, string phone){
	std::cout << "==============================" << std::endl;
	std::cout << "Profile for " << username << ": " << std::endl;
	std::cout << "Name: " << name << std::endl;
	std::cout << "Phone Number: " << phone << std::endl;
	std::cout << "==============================" << std::endl;

	
}

void printHex(string username, string name, string phone){
	char * msg = new char[name.size()+1];

	strcpy(msg, name.c_str());

	int n = strlen((const char*)msg);

	unsigned char * encryptedName = new unsigned char[n];
	for (int i = 0; i < n; i++) {
		encryptedName[i] = (unsigned char)msg[i];
	}	
	std::cout << "==============================" << std::endl;
	std::cout << "Profile for " << username << ": " << std::endl;
	std::cout << "Name: ";
	for (int i = 0; i < 16; i++) {
		std::cout << hex << (int) encryptedName[i];
		std::cout << " ";
	}
	std::cout << std::endl;

	//Now phone number
	char * msg2 = new char[phone.size()+1];

	strcpy(msg2, phone.c_str());

	int n2 = strlen((const char*)msg2);

	unsigned char * encryptedPhone = new unsigned char[n2];
	for (int i = 0; i < n2; i++) {
		encryptedPhone[i] = (unsigned char)msg2[i];
	}	
	std::cout << "Phone Number: ";
	for (int i = 0; i < 16; i++) {
		std::cout << hex << (int) encryptedPhone[i];
		std::cout << " ";
	}
	std::cout << std::endl;
	std::cout << "==============================" << std::endl;

	delete [] msg;
	delete [] msg2;
	delete [] encryptedName;
	delete [] encryptedPhone;


}

void Dictionary::printUser(string userName){ //This function is kind of useless. It will only print encrypted user info
	bool check = userSearch(userName); //Checks to see if the user exists first

	if (check == false){
		std::cout << "User " << userName << " does not exist!" << std::endl;
	}
	else{
		hashNode *tmp;
		long key = hash(userName);
		tmp = hTable[key];
		if (tmp->userName == userName){
			printHex(tmp->userName, tmp->name, tmp->phoneNumber);
		}
		else {
			while(tmp->next != nullptr){
				if (tmp->userName == userName){
					printHex(tmp->userName, tmp->name, tmp->phoneNumber);
				}
				tmp = tmp->next;
				if (tmp->userName == userName){
					printHex(tmp->userName, tmp->name, tmp->phoneNumber);
				}
			}
		}
	}
}

long Dictionary::hash(string key){
	long hash = 0;
	long index;
	long ascii = 0;

	for (int i = 0; i < key.length(); i++){
		if (i == 0){
			ascii = (int)key[i] * 128;
		}
		else{
			ascii = (int)key[i];
		}
		hash = hash + ascii;
	}
	index = (long)hash % tableSize;
	return index;
}

//Below methods are for user menu and user input
int Dictionary::menuSelection(){
	int selection;
	std::cout << "==============================" << std::endl;
	std::cout << "**Pick an Option**" << std::endl;
	std::cout << "(1) Add User" << std::endl;
	std::cout << "(2) Login" << std::endl;
	std::cout << "(3) Dev Menu" << std::endl;
	std::cout << "(4) Quit" << std::endl;
	std::cout << "Selection: ";

	std::cin >> selection;

	return selection;
}

int Dictionary::menuSelectionTwo(){ //Second menu once the user logs in
	int selection;
	std::cout << "==============================" << std::endl;
	std::cout << "**Pick an Option**" << std::endl;
	std::cout << "(1) Edit User" << std::endl;
	std::cout << "(2) Remove User" << std::endl;
	std::cout << "(3) Exit" << std::endl;
	std::cout << "Selection: ";

	std::cin >> selection;
	return selection;
}

void Dictionary::addUserUtil(){
	string username;
	string name;
	string phone;	
	unsigned char key[16];
	std::cout << "==============================" << std::endl;
	std::cout << "Enter Username: ";
	std::cin >> username;
	std::cout << "What is your name?:  ";
	std::cin >> name;
	std::cout << "Enter Phone Number: ";
	std::cin >> phone;
	
	std::cout << "Create a 16 Character Number Only Password: ";
	for (int i = 0; i < sizeof(key); i++){
		std::cin >> key[i];
	}

	addUser(username, name, phone, key);
}

bool Dictionary::loginUtil(string username, unsigned char *key){
	
	bool correctLogin = login(username, key); 
	return correctLogin;
}

void Dictionary::editUserUtil(Dictionary::hashNode &user, unsigned char *password){
	Aes aes;
	int selection;
	string newInfo;
	std::cout << "==============================" << std::endl;
	std::cout << "(1) Change Name" << std::endl;
	std::cout << "(2) Change Phone Number" << std::endl;
	std::cout << "Selection: ";
	std::cin >> selection;
	std::cout << "Enter new info: ";
	std::cin >> newInfo;

	if (selection == 1){
		string e = aes.encryptUtil(password, newInfo);
		user.name = e;
	}
	else if (selection == 2){
		string e = aes.encryptUtil(password, newInfo);
		user.phoneNumber = e;
	}
}

void Dictionary::editUser(string username, unsigned char *password){
	hashNode *tmp;
	long key = hash(username);
	tmp = hTable[key];
	if (tmp->userName == username){
		editUserUtil(*tmp, password);
		return;
	}
	else{
		while(tmp->next != nullptr){
			if (tmp->userName == username){
				editUserUtil(*tmp, password);
				return;
			}
			tmp = tmp->next;
			if (tmp->userName == username){
				editUserUtil(*tmp, password);
				return;
			}
		}
	}

}

void Dictionary::removeUserUtil(Dictionary::hashNode &user){
	std::cout << "User " << user.userName << " Removed!" << std::endl;

	user.userName = "emptyBucket";
	user.name = "emptyBucket";
	user.phoneNumber = "emptyBucket";
	user.password = "emptyBucket";
}

void Dictionary::removeUser(string username){
	hashNode *tmp;
	long key = hash(username);
	tmp = hTable[key];
	if (tmp->userName == username){
		removeUserUtil(*tmp);
		return;
	}
	else{
		while(tmp->next != nullptr){
			if (tmp->userName == username){
				removeUserUtil(*tmp);
				return;
			}
			tmp = tmp->next;
			if (tmp->userName == username){
				removeUserUtil(*tmp);
				return;
			}
		}
	}

}
