# Password Hashing with AES
Program implementing AES encrypted hash table

# Project README:

## b.) Design:

The whole project is separated into two pairs of .h/.cpp files. The encryption/decryption is in one pair, and the hash table is in the other. Then I used a separate main.cpp file. For the AES algorithms to work with the hash table, I first grab the user’s username, name, phone number, and password. All of this except the username is encrypted. Next, I find the hash value by hashing the clear text username which is not encrypted. Then all four inputs are put in the hash table after being converted to strings. When the user logs in, they have to enter their username and password. The program first encrypts the entered passwords, then searches for hash bucket with the matching username, and then compares the encrypted password to the one stored in the hash table. If both the username and the password match, then the login is successful.

When the user enters the password, the progam will not move onto the next step until all 16 numbers are entered. If the user presses enter with less than 16 numbers, a new line will start where they will have to finish typing their password. If the user types more than 16 numbers the password becomes the first 16 numbers they entered.


## c.) Time Analysis:

**Full results are in the “report” document**

To determine the speed of encryption and decryption, I tested both based on the number of words entered, and the size of the words entered. Both graphs for these tests grew linearly. Considering that the decryption and encryption are basically the inverse of each other, there was little to no difference in the speeds of them. Decryption showed to be slightly faster when testing on size of words in comparison to the encryption. My only explanation for this is because there is more preparation involved in the encryption. With encryption, the word being encrypted has to first be padded. Therefore, I believe the difference in time only showed with the size of the words because the padding was done on words very large in size. 

**Time Complexity:** Since both the encryption and decryption are working on fixed block size, they are usually O(1). However, when larger words are being encrypted, it can become O(m) where m is the size of the message.


## d.) How code could have been improved:

One thing I regret doing is not separating the decryption algorithm from the files with encryption. Since they both share similar methods, It got confusing at times. For instance, methods such as mix columns and inverse mix columns both looked identical even though one was the inverse of the other. Other than this, I felt as if the code is well organized. A feature that I wish I worked on implementing is the use of varying key sizes other than 16 bytes. The AES algorithms accepts 3 different byte sizes keys, however, I set up my program to only work with 16. My main issue with working with varying sized keys was that I was unable to grab the user input into strings like I was able to with everything else. Therefore, I would have had to ask the user what size password they wanted before they entered it. I chose not to do this because it is very unrealistic for any website or program to ask the user that.



