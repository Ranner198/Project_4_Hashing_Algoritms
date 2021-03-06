#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <string.h>

#define TABLESIZE     100
#define KEYSIZE       4
#define EMPTYKEY      "----"
#define DATAFILE      "P4DATA.TXT"

using namespace std;

// Define the structure for use in the hash table
struct HashStruct
{	
     char key[5];
     int data;
};

void InitTable(HashStruct hashT[], int TableSize)
{
    int i;
	
    for(i=0; i<TableSize; i++)
    {
        strcpy(hashT[i].key, EMPTYKEY);
        hashT[i].data = 0;
    }
}

int Hash_1(char *key) {
	int hashIndex = int(key[0] % TABLESIZE);
	return hashIndex;
}

int Hash_2(char *key) {
	int hashIndex = (int(key[2] + int(key[3])) % TABLESIZE);
	return hashIndex;
}

int Hash_3(char *key) {
	int hashIndex = int(key[2] % TABLESIZE);
	return hashIndex;
}

int ProbeDec_1 (char *key) {
	//Liner Probing Example "Nos Touches"
	int probeAmt = int((key[0] + 1) % TABLESIZE);
	return probeAmt;
}

int ProbeDec_2 (char *key) {
	//Base 26
	long index =  ((key[0]-'A')*pow(26,3)) +
		((key[1]-'A')*pow(26,2)) +
		((key[2]-'A')*26) +
		((key[3]-'A')) % TABLESIZE;
	return index;
}

int ProbeDec_3 (char *key) {
	long index = (key[1] + key[2] * key[1] + key[2]) % TABLESIZE;
	return index;
}



int HashInsert(HashStruct T[], char *key, int data, int hNum, int dhNum)
{
    int  testNum = (hNum * 3) + dhNum;
    int  colCount = 0;
    int  hashIndex, probeDec;

    switch(testNum)
    {
            case 0 :  // Hash function 1 -- Double hash 1 (linear probing) 
                    hashIndex = Hash_1(key);
                    probeDec = ProbeDec_1(key); // Function just returns 1 
                    break;
            case 1 :  // Hash function 1 -- Double hash 2  
                    hashIndex =Hash_1(key);
                    probeDec = ProbeDec_2(key);
                    break;
            case 2 :  // Hash function 1 -- Double hash 3  
                    hashIndex = Hash_1(key);
                    probeDec = ProbeDec_3(key);
                    break;
            case 3 :  // Hash function 2 -- Double hash 1 (linear probing)  
                    hashIndex = Hash_2(key);
                    probeDec = ProbeDec_1(key); // Function just returns 1
                    break;
            case 4 :  // Hash function 2 -- Double hash 2  
                    hashIndex = Hash_2(key);
                    probeDec = ProbeDec_2(key);
                    break;
            case 5 :  // Hash function 2 -- Double hash 3  
                    hashIndex = Hash_2(key);
                    probeDec = ProbeDec_3(key);
                    break;
            case 6 :  // Hash function 3 -- Double hash 1 (linear probing)  
                    hashIndex = Hash_3(key);
                    probeDec = ProbeDec_1(key); // Function just returns 1
                    break;
            case 7 :  // Hash function 3 -- Double hash 2  
                    hashIndex = Hash_3(key);
                    probeDec = ProbeDec_2(key);
                    break;
            case 8 :  // Hash function 3 -- Double hash 3  
                    hashIndex = Hash_3(key);
                    probeDec = ProbeDec_3(key);
					//cout<<"Key: " << key << endl;
                    break;
    }

	// Find a place to insert into the table
	int maxTimeOut = 0;

	while(strcmp(T[hashIndex].key, EMPTYKEY) != 0)
	{
		
		//Incase of a hang-up
		if (maxTimeOut > 20) 
			probeDec = 1;
		
		colCount++;
		hashIndex += probeDec;

		if(hashIndex > TABLESIZE)
			hashIndex = 0;

		maxTimeOut++;
	}

	strcpy(T[hashIndex].key, key);

	return colCount;
}

int main() {	

	 //1
	 //==============================================================================
     int          i, hashNum, dHashNum, count;
     ifstream     *inFile;
     HashStruct   T[100];  // Hash table array of 100 data structures
     char         line[64];// Array to hold lines read from file
     char         key[5];  // Array to hold 4-character keys
     int          data;    // Integer data

	  //2
	  // For each of three hash functions
      for(hashNum = 0; hashNum < 3; hashNum++)
      {
		 cout << "\n\n*****************************************  Current Set: " << hashNum+1 << "  **************************************************\n\n" << endl;
         // For each of three double hash functions
         for(dHashNum = 0; dHashNum < 3; dHashNum++)
         {
            InitTable(T, TABLESIZE);               // Call function to initialize table to empty
            inFile = new ifstream();
            inFile->open("C:/Users/rancr/Desktop/Project_4_Hashing_Algoritms-master/P4Data.txt", ifstream::in);   // Open data file for this test
            if(!inFile->is_open())
            {
               cout << "Unable to open data file. Program terminating\n";
			   system("pause");
               return 0;
            }
            count = 0;     // Initialize collision counter
            for(int i = 0; i < 50; i++)
            {
                 inFile->getline(line, 64, '\n');
                 sscanf(line, "%s %d", key, &data);
                 count += HashInsert(T, key, data, hashNum, dHashNum);
                 // Note: The HashInsert() function uses the indices of hashNum and
                 // dHashNum to know which hash function and double hash function to call
                 // in this test.
            }
            inFile->close();		/* Close the text file */
            delete inFile;

			//3
			cout << "Testing hash function " << hashNum+1 << " using double hash " << dHashNum+1 << ".\n";
			
			switch(dHashNum+1) {

			case 1: cout << "Hash function type: Liner Probing" << endl;
				break;
				
			case 2: cout << "Hash function type: Base-26" << endl;
				break;

			case 3: cout << "Hash function type: Middle Squaring" << endl;
				break;
			}

			cout << "Total collisions = " << count << ".\n";
			// Show the form of the array
			for(int i=0; i < 100; i++)
				if(strcmp(T[i].key, EMPTYKEY)) // strcmp gives a non-zero (true) result if Key is NOT the EMPTYKEY
					cout << "|";     // Indicate an entry at this location
				else
					cout << "-";     // Indicate no entry at this location
			cout << "\n\n";
		 }
      }

	system("pause");
	return 0;
}
