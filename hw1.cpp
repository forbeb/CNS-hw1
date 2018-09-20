#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <string>
#include <sstream>

using namespace std;

int* permutation(int* text, int* key, int size){
//permutates a set of integers to positions signified by the key
	int* new_text = new int[size];
	for(int i=0; i<size; ++i){
		new_text[i] = text[key[i]-1];
	}
	return new_text;
}

int** split(int key[], int size){
//takes an array and returns two arrays for each half
	int** new_key = new int*[2];
	new_key[0] = new int[size/2];
	new_key[1] = new int[size/2];
	for(int i=0; i<size; ++i){
		new_key[i/(size/2)][i%(size/2)]=key[i];
	}
	return new_key;
}

int* concatenate(int* one, int* two, int size){
//concatenates two arrays
	int* new_key = new int[size*2];
	for(int i=0; i<size; ++i){
		new_key[i]=one[i];
	}
	for(int i=0; i<size; ++i){
		new_key[i+size]=two[i];
	}
	return new_key;
}

int* xxor(int* one, int* two, int size){
	//performs the xor between two arrays

	int* x = new int[size];
	for(int i=0; i<size; ++i){
		x[i] = one[i]^two[i];
	}

	return x;
}

///////////////////////

int* left_shift(int key[5]){
	//shifts all elements in an array left
	int tmp = key[0];
	for(int i=0; i<4; ++i){
		key[i]=key[i+1];
	}
	key[4]=tmp;
	return key;
}

int** create_k(int init_key[10], int ten_bt_perm[10], int eight_bt_perm[8], int num_k){
	//creates our k-keys

	int** k = new int*[num_k];//the k-keys

	int* p10 = permutation(init_key, ten_bt_perm, 10);//the text after p10 permutation
	int** five_bit = split(p10,10);//split p10

	for(int i=0; i<num_k; ++i){
		//left shift the split array
		five_bit[0] = left_shift(five_bit[0]);
		five_bit[1] = left_shift(five_bit[1]);

		delete[] p10;
		//concatanate
		p10=concatenate(five_bit[0], five_bit[1], 5);

		//add new k-key to our k-keys
		k[i] = permutation(p10, eight_bt_perm, 8);
	}

	delete[] p10;
	delete[] five_bit[0];
	delete[] five_bit[1];
	delete[] five_bit;
	
	return k;
}
////////////////////////////////

int* sub(int table[4][4][2], int bits[4]){
	//apply substitution matrix
	int row = 2*bits[0]+bits[3];
	int col = 2*bits[1]+bits[2];

	return table[row][col];
}

int* f_function(int four_bit[4], int eight_bit_key[8], int expanse_key[8], int four_perm_key[4]){
	//the f-function

	//expansion/permutation
	int* eight_bits = permutation(four_bit, expanse_key, 8);

	int* post_xor = xxor(eight_bits, eight_bit_key, 8);//applies xor
	delete[] eight_bits;

	int** split_bits = split(post_xor, 8);//splits new text
	delete[] post_xor;


	//our matrix tables
	int table_1[4][4][2]={ 	{{0,1},{0,0},{1,1},{1,0}},
							{{1,1},{1,0},{0,1},{0,0}},
							{{0,0},{1,0},{0,1},{1,1}},
							{{1,1},{0,1},{1,1},{1,0}}	};

	int table_2[4][4][2]={ 	{{0,0},{0,1},{1,0},{1,1}},
							{{1,0},{0,0},{0,1},{1,1}},
							{{1,1},{0,0},{0,1},{0,0}},
							{{1,0},{0,1},{0,0},{1,1}}	};
	//matrix substitution
	int* two_bit_one = sub(table_1, split_bits[0]);
	int* two_bit_two = sub(table_2, split_bits[1]);

	int* p4 = concatenate(two_bit_one, two_bit_two, 2);
	//concatenate then permutate then return
	int* final = permutation(p4, four_perm_key, 4);
	delete[] p4;

	return final;
}
//////////////////////////
int* encrypt(int text[8], int init_perm[8],int inv_perm[8], int rounds, int** k, int expanse_key[8], int four_key[4]){
	//our DES encryptor

	int* new_text = permutation(text, init_perm, 8); // initial permutation

	int** split_text = split(new_text, 8);//split into four bits

	int* left = split_text[0];
	int* right = split_text[1];

	for(int i=0; i<rounds; ++i){//begin DES loop
		int* f = f_function(right, k[i], expanse_key, four_key);//apply f-function

		int* new_left = xxor(left, f, 4);//xor
		delete[] left;
		//switch left and right
		left = right;
		right = new_left;
	}

	int* final = concatenate(right, left, 4);//concatenate final four bits

	int* final_perm = permutation(final, inv_perm, 8);//reverse initial permutation

	delete[] left;
	delete[] right;
	delete[] split_text;
	delete[] final;

	return final_perm;
}
////////////////////////////////

int* decrypt(int text[8], int init_perm[8],int inv_perm[8], int rounds, int** k, int expanse_key[8], int four_key[4]){
	//this is exactly the same as encrypt except the k keys are applied in reverse order


	int* new_text = permutation(text, init_perm, 8);

	int** split_text = split(new_text, 8);

	int* left = split_text[0];
	int* right = split_text[1];

	for(int i=rounds-1; i>=0; --i){
		int* f = f_function(right, k[i], expanse_key, four_key);

		int* new_left = xxor(left, f, 4);
		delete[] left;

		left = right;
		right = new_left;
	}

	int* final = concatenate(right, left, 4);

	int* final_perm = permutation(final, inv_perm, 8);

	delete[] left;
	delete[] right;
	delete[] split_text;
	delete[] final;

	return final_perm;

}

int* vec_to_arr(vector<int> v){//convert vector to array
	int* a = new int[8];
	for(int i = 0; i<8; ++i){
		a[i] = v[i];
	}
	return a;
}

vector<int> arr_to_vec(int a[8]){//convert array to vector
	vector<int> v;
	for(int i=0; i<8; ++i){
		v.push_back(a[i]);
	}
	return v;
}

int main(int argc, char* argv[]){
	
	if (argc != 2) {
		std::cerr << "Wrong number of arguments. I dont want to do this anymore. " << std::endl;
		return 1;
	}

	ifstream in_str(argv[1], ios_base::binary);

	char readLine;
	vector<char> words;//chars represented as chars ya dingus

	while(1){//read in file and represent as vector of chars
		readLine = in_str.get();
		if(!in_str.good())
			break;
		words.push_back(readLine);

	}

	vector<vector<int> > chars;//chars represented as 8 bit binary numbers
	vector<bitset<8> > bits; //chars represented as bitsets

	for(unsigned int i=0; i<words.size(); i++){//convert chars to 8-bitset
		bitset<8> b(words[i]);
		bits.push_back(b);
	}


	for(unsigned int i=0; i<bits.size(); i++){//convert 8bitsets to double vector of int
		vector<int> v;
		chars.push_back(v);
		for(int j=0; j<8; ++j){
			chars[i].push_back(bits[i][7-j]);
		}
	}



	//kets needed
	int ten_bit_init_key[10] = {1,0,0,0,1,1,1,0,1,0};
	int ten_bit_perm[10] = {3,5,2,7,4,10,1,9,8,6};

	int init_perm_key[8] = {2,6,3,1,4,8,5,7};
	int inv_perm_key[8] = {4,1,3,5,7,2,8,6};
	int eight_perm_key[8] = {6,3,7,4,8,5,10,9};
	int four_perm_key[4] = {2,4,3,1};		
	int expanse_key[8] = {4,1,2,3,2,3,4,1};

	//create the k-keys
	int** k = create_k(ten_bit_init_key, ten_bit_perm, eight_perm_key, 2);


	vector<vector<int> > encryption;
	//create encryption
	for(unsigned i=0; i<chars.size(); ++i){
		int* cipher = encrypt(vec_to_arr(chars[i]) , init_perm_key, inv_perm_key, 2, k, expanse_key, four_perm_key);

		encryption.push_back(arr_to_vec(cipher));
	}

	//create decryption
	vector<vector<int> > decryption;
	for(unsigned i=0; i<encryption.size(); ++i){
		int* decipher = decrypt(vec_to_arr(encryption[i]) , init_perm_key, inv_perm_key, 2, k, expanse_key, four_perm_key);

		decryption.push_back(arr_to_vec(decipher));
	}

	string encryption_string;
	//convert encryption from ints to chars
	for(unsigned i=0; i<encryption.size(); ++i){
		string tmp;
		string tmp2;
		for(int j=0; j<8; ++j){
			stringstream ss;
			ss << encryption[i][j];
			tmp2 = ss.str();
			tmp = tmp + tmp2;
		}

		bitset<8> b(tmp);
		encryption_string = encryption_string + char(b.to_ulong());
	}

	string output;
	//convert decryption from ints to chars
	for(unsigned i=0; i<decryption.size(); ++i){
		string tmp;
		string tmp2;
		for(int j=0; j<8; ++j){
			stringstream ss;
			ss << decryption[i][j];
			tmp2 = ss.str();
			tmp = tmp + tmp2;
		}

		bitset<8> b(tmp);
		output = output + char(b.to_ulong());
	}

	//output input
	cout<<"plaintext: ";
	for(unsigned i=0; i<words.size(); ++i){
			cout<<words[i];
	}
	cout<<endl;
	//output encryption as chars
	cout<<"encryption: "<<encryption_string<<endl;
	//output decryption as chars
	cout<<"decryption: "<<output<<endl;


	// delete[] cipher;
	// delete[] new_text;      
	delete[] k[0];
	delete[] k[1];
	delete[] k;
}