#ifndef Buffer_Manager_h
#define Buffer_Manager_h

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#define MAX_FILE_NUM 100
#define MAX_BLOCK_NUM 30
#define BLOCK_SIZE_FILE 1
#define FILE_SIZE 8192
#define INITIALNUM 0
#define ROOT_PATH "Data/"
using namespace std;


struct fileNode {
	string *fileName;
	int pagenum;
	fileNode* prefile;
	fileNode* nextfile;
	bool blocksign;
	int file_block;
	int written;
};
struct blockNode {
	char* block;
	bool pined;
	int count;
	fileNode* block_file;
	int reference;
};
class BufferManager
{
private:
	fileNode file_pool[MAX_FILE_NUM];
	blockNode block_pool[MAX_BLOCK_NUM];
	int total_block; // the number of block that have been used, which means the block is in the list.
	int total_file; // the number of file that have been used, which means the file is in the list.
	int indexoffreefile;
	int badblock;
	bool findthebadblock();
	void init_block(blockNode & block);
	void init_file(fileNode & file);
	fileNode* find(string file, int offest);
public:
	BufferManager(int filenum = 10, int blocknum = 3);
	~BufferManager();
	char* readfromfile(string file, int offest);//�ļ����͵ڼ�����
	bool writetofile(string file, int offest, int index, char* data, int length);
	//fileΪ��.txt��׺���ļ�����index�ǵڼ����ַ�
	char* read(string file, int index);
	bool write(string file, int index, char* data, int length);//���ڴ�д��������
	bool write(string file, int index);//ʹ�����޸ĺ�block�����ú���������̣��ļ������ڿ���
	bool freefile(string file);//ɾ���ļ�
	bool setpin(string file, int index);//�����������ļ������ڿ���
	bool freepin(string file, int index);//�ͷ��������ļ������ڿ���
};
extern BufferManager bm;

#endif 
