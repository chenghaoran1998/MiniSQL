#include "Buffer_Manager.h"
BufferManager bm;


BufferManager::BufferManager(int filenum, int blocknum) {
	total_file = filenum;
	total_block = blocknum;
	indexoffreefile = 0;
	badblock = 0;
	for (int i = 0; i<total_file; i++) {
		init_file(file_pool[i]);
	}
	for (int i = 0; i<total_block; i++) {
		init_block(block_pool[i]);
	}
}
BufferManager::~BufferManager() {
	for (int i = 0; i<total_block; i++) {
		free(block_pool[i].block);
	}
}
void BufferManager::init_block(blockNode & block) {
	block.block = (char*)malloc(sizeof(char)*FILE_SIZE*BLOCK_SIZE_FILE);
	block.count = 0;
	block.block_file = nullptr;
	block.reference = 0;
	block.pined = false;
}
void BufferManager::init_file(fileNode & file) {
	file.fileName = new string;
	*file.fileName = "false";
	file.pagenum = 0;
	file.prefile = nullptr;
	file.nextfile = nullptr;
	file.blocksign = false;
	file.file_block = -1;
	file.written = false;
}
bool BufferManager::findthebadblock() {
	int index = -1, minofrefer = 99999;
	for (int i = 0; i<total_block; i++) {
		if (block_pool[i].reference<minofrefer&&block_pool[i].pined == false) {
			index = i;
			minofrefer = block_pool[i].reference;
		}
	}
	if (index >= 0 && index<total_block) {
		badblock = index;
		return true;
	}
	else {
		cout << "find the bad block fail" << endl;
		return false;
	}
}
fileNode* BufferManager::find(string file, int offest) {
	fileNode* ftemp = nullptr;
	for (int i = 0; i<total_file; i++) {
		if (file == *file_pool[i].fileName) {
			ftemp = &file_pool[i];
			break;
		}
	}
	if (ftemp == nullptr) {
		ftemp = &file_pool[indexoffreefile];
		string path = ROOT_PATH + file + to_string(0) + ".txt";
		fstream f0(path, ios::in);
		if (!f0.good()) {
			fstream f(path, ios::out);
			char initialnum = INITIALNUM;
			for (int j = 0; j<FILE_SIZE; j++) {
				f << initialnum;
			}
			f.close();
		}
		f0.close();
		////for debug
		//cout << "write without prefile in :" << path << endl;

		*file_pool[indexoffreefile].fileName = file;
		file_pool[indexoffreefile].pagenum = 0;
		file_pool[indexoffreefile].written = true;
		int i;
		for (i = 0; i<total_file; i++) {//文件空间管理，可以链表优化
			if (file_pool[i].written == 0) {
				indexoffreefile = i;
				break;
			}
		}
		if (i == total_file) {
			cout << "lack in file size";
			return nullptr;
		}
	}
	if (ftemp->pagenum < offest) {
		while (ftemp->pagenum<offest) {
			if (ftemp->nextfile == nullptr) {
				string path = ROOT_PATH + file + to_string(ftemp->pagenum + 1) + ".txt";
				fstream f0(path, ios::in);
				if (!f0.good()) {
					fstream f(path, ios::out);
					char initialnum = INITIALNUM;
					for (int j = 0; j<FILE_SIZE; j++) {
						f << initialnum;
					}
					f.close();
				}
				f0.close();
				////for debug
				//cout << "write with sequence in :" << path << endl;
			}
			ftemp->nextfile = (fileNode*)malloc(sizeof(fileNode));
			init_file(*ftemp->nextfile);
			*ftemp->nextfile->fileName = file;
			ftemp->nextfile->pagenum = ftemp->pagenum + 1;
			ftemp->nextfile->written = true;
			ftemp->nextfile->prefile = ftemp;
			ftemp = ftemp->nextfile;
		}
	}
	return ftemp;
}
char* BufferManager::readfromfile(string file, int offest) {
	if (offest<0) {
		cout << "you give the illegal offset in readfile" << endl;
		return nullptr;
	}
	fileNode* ftemp = find(file, offest);
	if (ftemp->blocksign == true) {
		block_pool[ftemp->file_block].reference++;
		return block_pool[ftemp->file_block].block;
	}
	else {
		if (!findthebadblock()) {
			return nullptr;
		}

		if (block_pool[badblock].block_file != nullptr) {
			block_pool[badblock].block_file->blocksign = false;
		}
		ftemp->blocksign = true;
		ftemp->file_block = badblock;
		block_pool[badblock].block_file = ftemp;
		char* p = block_pool[badblock].block;
		string path = ROOT_PATH + file + to_string(offest) + ".txt";
		fstream f(path, ios::in | ios ::binary);
		for (int j = 0; j<FILE_SIZE; j++) {
			char n;
			f.get(n);
			p[j] = n;
		}
		f.close();
		for (int j = 0; j<total_block; j++) {
			block_pool[j].reference--;
		}
		////for debug
		//for (int i = 0; i<8; i++) {
		//	cout << p[i] << " ";
		//}
		//cout << endl;

		block_pool[badblock].reference = 1;
		return p;
	}
}
bool BufferManager::writetofile(string file, int offest, int index, char* data, int length) {
	if (offest<0) {
		cout << "you give the illegal offset in writefile" << endl;
		return false;
	}
	fileNode* ftemp = find(file, offest);
	if (ftemp->blocksign == false) {
		if (!findthebadblock()) {
			return false;
		}
		if (block_pool[badblock].block_file != nullptr) {
			block_pool[badblock].block_file->blocksign = false;
		}
		ftemp->blocksign = true;
		ftemp->file_block = badblock;
		block_pool[badblock].block_file = ftemp;
		char* p = block_pool[badblock].block;
		string path = ROOT_PATH + file + to_string(offest) + ".txt";
		////for debug
		//cout << "read to :" + to_string(badblock) + " block in " + path << endl;

		fstream f(path, ios::in | ios::binary);
		for (int j = 0; j<FILE_SIZE; j++) {
			char n;
			f.get(n);
			p[j] = n;
		}
		f.close();
		for (int j = 0; j<total_block; j++) {
			block_pool[j].reference--;
		}
		block_pool[badblock].reference = 1;
	}
	memcpy(block_pool[ftemp->file_block].block + index, data, length);
	string path = ROOT_PATH + file + to_string(offest) + ".txt";
	fstream f(path, ios::in + ios::out + ios::binary);
	f.seekp(index, ios::beg);
	for (int i = 0; i<length; i++) {
		f.put(data[i]);
	}
	////for debug
	//f.seekg(0, ios::beg);
	//for (int i = 0; i<length + 5; i++) {
	//	char n;
	//	f >> n;
	//	cout << n << " ";
	//}
	//cout << endl;

	f.close();
	return true;
}

char* BufferManager::read(string file, int index) {
	int offset = index / FILE_SIZE;
	index -= offset * FILE_SIZE;
	char* data = readfromfile(file, offset) + index;
	return data;
}
bool BufferManager::write(string file, int index, char* data, int length) {
	int offset = index / FILE_SIZE;
	index -= offset * FILE_SIZE;
	bool success = writetofile(file, offset, index, data, length);
	return success;
}
// work only the page of file in the blocks!!!
bool BufferManager::write(string file, int index) {
	int offset = index / FILE_SIZE;
	for (int i = 0; i<total_block; i++) {
		if (*block_pool[i].block_file->fileName == file && offset == block_pool[i].block_file->pagenum) {
			char* p = block_pool[i].block;
			string path = ROOT_PATH + file + to_string(offset) + ".txt";
			fstream f(path, ios::in + ios::out +ios::binary);
			for (int i = 0; i<total_block; i++) {
				f << p[i];
			}
			f.close();
			return true;
		}
	}
	return false;
}
bool BufferManager::freefile(string file) {
	fileNode* ftemp = find(file, 0);
	int i = 0;
	while (ftemp != nullptr) {
		string path = ROOT_PATH + *ftemp->fileName + to_string(i) + ".txt";
		unlink(path.c_str());
		fileNode* temp = ftemp->nextfile;
		if (i == 0) {
			init_file(*(ftemp));
		}
		else {
			free(ftemp);
		}
		ftemp = temp;
		i++;
	}
	return true;
}
// work only the page of file in the blocks!!!
bool BufferManager::setpin(string file, int index) {
	int offset = index / FILE_SIZE;
	for (int i = 0; i<total_block; i++) {
		if (*block_pool[i].block_file->fileName == file && offset == block_pool[i].block_file->pagenum) {
			block_pool[i].pined = true;
			return true;
		}
	}
	return false;
}
bool BufferManager::freepin(string file, int index) {
	int offset = index / FILE_SIZE;
	for (int i = 0; i<total_block; i++) {
		if (*block_pool[i].block_file->fileName == file && offset == block_pool[i].block_file->pagenum) {
			block_pool[i].pined = false;
			return true;
		}
	}
	return false;
}

