#include <cstdio>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#define MAX_FILE_NUM 100
#define MAX_BLOCK_NUM 30
#define BLOCK_SIZE_FILE 1
#define FILE_SIZE 8192
#define INITIALNUM 0

using namespace std;

int main ()
{
//	FILE * fp = fopen("author0.txt","r+"); 
		fstream f("author0.txt", ios::in | ios::binary);
		for (int j = 0; j<FILE_SIZE; j++) {
			char n;
			f.get(n);
		cout << n << "|";
		}
		f.close();
} 
