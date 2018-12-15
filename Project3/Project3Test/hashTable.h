#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string>

class Dictionary{ //Stores each link of the hash table
	private:
		static const int tableSize = 20;
		struct hashNode{
			std::string userName; //Word
			std::string name;
			std::string phoneNumber;
			std::string password;
			int key; 
			hashNode* next;
		};

		hashNode* hTable[tableSize];

	public:
		Dictionary();
		long hash(std::string key);	//gets hash value
		void addUser(std::string userName, std::string name, std::string phoneNumber, unsigned char *password);//calls hash and inserts at that index
		int indexSize(int index);
		void printTable();
		void createDict(); //reads from english.txt
		bool userSearch(std::string userName); //uses hash index and finds word
		void printUserUtil(std::string, std::string, std::string);
		void printUser(std::string userName);
		bool login(std::string userName, unsigned char *password);

		//Below are menu selection utils
		int menuSelection();
		int menuSelectionTwo();
		void addUserUtil();
		bool loginUtil(std::string username, unsigned char *key);
		void editUserUtil(hashNode &user, unsigned char *password);
		void editUser(std::string username, unsigned char *password);
		void removeUserUtil(hashNode &user);
		void removeUser(std::string username);
};

#endif
