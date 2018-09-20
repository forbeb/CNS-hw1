README

Brendan Forbes
Cryptography and Network Security
Homework 1

The program takes a text file, reads it in an stores is as a vector of chars. It then converts it to a vector of binary bitsets and then a vector of vector of ints. The program then takes each character now represented as 8-bit binary numbers and begins the DES encryption process for each 8-bit binary number.

The DES encryptor permutates the 8-bit number, then splits it into two halves. The second half is fed into the f-function.

The f-function takes the 4-bit binary number, doubles and permutates it. It then xor's it with a k-key corresponding to the DES round taking place. The result is split into two and sent through two sperate matrix substitutions that result in two, two-bit numbers that are concatenated and spit out by the f-function.

The end result of the f-function xor's with the first half of the 8-bit number. The result becomes the second half for the next round of DES. The first half for the next round of DES is always the second half from the previous round.

After the final round, the two halves are concatanated and an inverse of the original permuation occurs. The result is an encryption of the 8-bit number.

The main section of code runs an encryption and decryption of the text. The decyption is the same as the encryption except the k-keys are used in reverse order. 

The program must be run with a txt file as an argument.