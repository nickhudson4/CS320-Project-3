#include <iostream>
#include <chrono>
#include <string.h>
#include <cstring>
#include <sstream>

#include "aes.h"
#include "hashTable.h"

//TODO: Delete dynamically
//Test 

using namespace std;

int main(){
	Aes aes;
	Dictionary dict;
	/* unsigned char key[16] =	{1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7}; */	
	/* unsigned char key2[16] = {1, 0, 3, 0, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; */
	/* unsigned char key3[16] = {1, 0, 0, 0, 0, 0, 4, 0, 0, 6, 0, 0, 0, 0, 9, 0}; */
	/* unsigned char key4[16] = {1, 0, 0, 0, 0, 4, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0}; */
	unsigned char key5[16] = {1, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 9, 0};

	/* dict.addUser("nickhudson4", "Nick", "3025194424", key); */
	/* dict.addUser("nickhudson5", "Jack", "3025195555", key2); */
	/* dict.addUser("nickhudson6", "Nicholas", "3025190285", key3); */
	/* dict.addUser("nickhudson7", "ANthony", "3024564424", key4); */
	dict.addUser("nickhudson8", "Joe", "3025123122", key5);
	float dTime = 0;

	for (int i = 1; i < 500000; i += 50000){
		for (int j = 0; j < i; j++){

			auto start2 = std::chrono::system_clock::now();
			bool check = dict.loginUtil("nickhudson8", key5);
			auto end2 = std::chrono::system_clock::now();
			std::chrono::duration<double> elapsed_seconds2 = end2-start2;
			std::time_t end_time2 = std::chrono::system_clock::to_time_t(end2);
			dTime += elapsed_seconds2.count();
		}

		
		std::cout << "Time to login " << i << " users: " << dTime << " seconds" << std::endl; 

	}


	/* int menuSelection; */ 
	/* while (menuSelection != 4){ */
	/* 	int menuSelection2 = 1; */
	/* 	menuSelection = dict.menuSelection(); */
	/* 	if (menuSelection == 1){ //Add user */
	/* 		dict.addUserUtil(); */
	/* 	} */
	/* 	else if (menuSelection == 2){ //Login */
	/* 		string username; */
	/* 		std::cout << "==============================" << std::endl; */
	/* 		std::cout << "Enter Username: "; */
	/* 		std::cin >> username; */
	/* 		unsigned char key[16]; */
	/* 		std::cout << "Enter your 16 Character Number Only Password: "; */
	/* 		for (int i = 0; i < sizeof(key); i++){ */
	/* 			std::cin >> key[i]; */
	/* 		} */

	/* 		bool check = dict.loginUtil(username, key); //Logs user in if username/password is correct */
	/* 		if (check == true){ */
	/* 			while (menuSelection2 != 3){ */
	/* 				menuSelection2 = dict.menuSelectionTwo(); */
	/* 				if (menuSelection2 == 1){ //Edit user */
	/* 					dict.editUser(username, key); */
	/* 				} */
	/* 				else if(menuSelection2 == 2){ //Remove user */
	/* 					dict.removeUser(username); */
	/* 				} */
	/* 			} */
	/* 		} */
	/* 	} */
	/* 	else if (menuSelection == 3){ */
	/* 		int devSelection; */
	/* 		std::cout << "(1) View Encrypted Profile" << std::endl; */
	/* 		std::cout << "(2) Timed Encrypt" << std::endl; */
	/* 		std::cout << "Selection: "; */
	/* 		std::cin >> devSelection; */
	/* 		if (devSelection == 1){ */
	/* 			string user; */
	/* 			std::cout << "Enter Username to View: "; */
	/* 			std::cin >> user; */
	/* 			dict.printUser(user); */
	/* 		} */
	/* 		else if (devSelection == 2){ */
	/* 			for (int i = 10000; i < 800000; i += 40000){ */
	/* 				aes.timedEncryptNumWords(i); */	
	/* 			} */

	/* 			std::cout << std::endl; */

	/* 			for (int i = 10000; i < 800000; i += 40000){ */
	/* 				aes.timedEncryptSizeWords(i); */
	/* 			} */
	/* 		} */
	/* 	} */
	/* } */
	return 0;
}
