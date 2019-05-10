//
//CSE461 File Systems Project
//Implementation of a simulated disk drive(Sdisk), a simple file system(Filesys),
//shell commands, and a single table database system.
//
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

class Sdisk{
public:
        Sdisk(string diskname, int numberofblocks, int blocksize);
	int getblock(int blocknumber, string& buffer);
        int putblock(int blocknumber, string buffer);
        int getnumberofblocks();
        int getblocksize();
        string getdiskname();
private:
        string diskname;
        int numberofblocks;
        int blocksize;
};

class Filesys:public Sdisk
{
public :
	Filesys(string filename, int numberofblocks, int blocksize);
	int fsclose();
	int fssynch();
	int newfile(string file);
	int rmfile(string file);
	int getfirstblock(string file);
	int addblock(string file, string buffer);
	int delblock(string file, int blocknumber);
	int readblock(string file, int blocknumber, string& buffer);
	int writeblock(string file, int blocknumber, string buffer);	
	int nextblock(string file, int blocknumber);
    	vector<string> block(string buffer, int b);
	vector<string> ls();

private :
    	int rootsize;
	int fatsize;
	vector<string> filename;
	vector<int> firstblock;
	vector<int> fat;
};



class Table:public Filesys{
public:
	Table(string filename,int numberofblocks,int blocksize,string flatfile,string indexfile);
	int Build_Table(string input_file);
	int Search(string value);

private:
	string flatfile;
	string indexfile;
	int numberofrecords;
	int IndexSearch(string value);
};	
		
		
		
		int main(){
	Table table("disk1",256,128,"flatfile","indexfile");
	table.Build_Table("data.txt");
	string date;
	cout<<"Enter date: ";
	cin>>date;
	cout<<endl;
	cout<<"Searching for "<<date<<endl;
	table.Search(date);

 	return 0;
}	

	

		
		
		
		
		
Sdisk::Sdisk(string diskname, int numberofblocks, int blocksize)
{
     this->diskname = diskname;
     this->numberofblocks = numberofblocks;
     this->blocksize = blocksize;
     fstream iofile;
     iofile.open(diskname.c_str(), ios::in);
     if(iofile.good())
     {
          cout << "File exists" << endl;
     } 
	else
     {
          cout << "File does not exist. Creating file " << diskname << " " << endl;
          iofile.open(diskname.c_str(), ios::out);
          for(int i=0; i<(blocksize*numberofblocks); i++)
          {
               iofile.put('#');
          }

     }
     iofile.close();
}
int Sdisk::getblock(int blocknumber, string& buffer)
{
	fstream infile;
	infile.open(diskname.c_str(), ios::in);
	infile.seekg(blocknumber*blocksize);
	if(infile.good()){
        	for(int i=0; i<blocksize; i++){
                	buffer += infile.get();
          	}
          	infile.close();
          	return 1;
     	} 
	else{
        	return 0;
     	}
}

int Sdisk::putblock(int blocknumber, string buffer)
{
     fstream outfile;
     outfile.open(diskname.c_str(), ios::in | ios::out);
     outfile.seekg(blocknumber*blocksize);
     if(outfile.good())
     {
          for(int i=0; i<buffer.length() && i<blocksize; i++)
          {
               outfile.put(buffer[i]);
          }
          outfile.close();
          return 1;
     }
     else
     {
          cout << "failed to write to file" << diskname << " " << endl;
          return 0;
     }
}

int Sdisk::getnumberofblocks(){
	return numberofblocks;
}

int Sdisk::getblocksize(){
	return blocksize;
}

string Sdisk::getdiskname()
{
     return diskname;
}

Filesys::Filesys(string file, int numberofblocks, int blocksize):Sdisk(file,numberofblocks,blocksize){
    string buffer;
	rootsize = getblocksize()/12;
	fatsize = (getnumberofblocks()*5)/(getblocksize())+1;  
	getblock(1,buffer);
	if(buffer[1]=='#'){
                cout<<"No File System"<<endl;
		for(int i=0; i<=rootsize; i++){
                	filename.push_back("XXXXX");
                        firstblock.push_back(0);
                }
		fat.push_back(fatsize+2);
                for(int i=1; i<=fatsize + 1; i++){
                        fat.push_back(1);
                }
                for(int i=2+fatsize; i<getnumberofblocks(); i++){
                        fat.push_back(i+1);
                }
                fat[fat.size()-1] = 0;
		cout<<"Created File System"<<endl;
       }
       else{
                cout<<"File System Exists"<<endl;
		string buffer;
        	istringstream readroot;
        	readroot >> buffer;
		for(int i = 0; i < rootsize; i++){
			string s;
        		int n;
            		readroot >> s >> n;
            		filename.push_back(s);
            		firstblock.push_back(n);
        	}
		string fatbuffer;
        	for(int i = 0; i < fatsize; i++){
            		string s;
            		getblock(i + 2, s);
            		fatbuffer += s;
        	}
        	istringstream fatstream;
        	fatstream.str(fatbuffer);
		for(int i = 0; i < getnumberofblocks(); i++){
            		int temp;
            		fatstream >> temp;
            		fat.push_back(temp);
		}
	}
	fssynch();
}

vector<string> Filesys::block(string buffer, int b){
    vector<string> blocks;
    int numberofblocks=0;

    if (buffer.length() % b == 0){
        numberofblocks= buffer.length()/b;
    }
    else{
        numberofblocks= buffer.length()/b +1;
    }

    string tempblock;
    for (int i=0; i<numberofblocks; i++){
        tempblock=buffer.substr(b*i,b);
        blocks.push_back(tempblock);
    }

    int lastblock=blocks.size()-1;

    for (int i=blocks[lastblock].length(); i<b; i++){
        blocks[lastblock]+="#";
    }

    return blocks;
}

vector<string> block(string buffer, int b){
  vector<string> blocks;
  int numberofblocks=0;

  if (buffer.length() % b == 0){
    numberofblocks= buffer.length()/b;
  }
  else{
    numberofblocks= buffer.length()/b +1;
  }

  string tempblock;
  for (int i=0; i<numberofblocks; i++){
    tempblock=buffer.substr(b*i,b);
    blocks.push_back(tempblock);
  }

  int lastblock=blocks.size()-1;

  for (int i=blocks[lastblock].length(); i<b; i++){
    blocks[lastblock]+="#";
  }

  return blocks;
}

int Filesys::fsclose(){
	fssynch();
	return 0;
}

int Filesys::fssynch(){
	ostringstream rootstream;
    
    	for(int i = 0; i < rootsize; ++i){
        	rootstream << filename[i] << " " << firstblock[i] << " ";  
    	}	
    	string root = rootstream.str();
    	for(long j = root.size(); j < getblocksize(); j++){
        	root += "#";
    	}
    	putblock(1, root);
    
    	ostringstream fatstream;
    
    	for(int i = 0; i < getnumberofblocks(); i++){
        	fatstream << fat[i] << " ";
    	}
    	string fatstring = fatstream.str();
    	vector<string> blocks=block(fatstring,getblocksize());
    
    	for(int i = 0; i < blocks.size(); i++){
        	putblock(i+2, blocks[i]);
       	}
    	return 1; 
}

int Filesys::newfile(string file){
        for(int i=0; i<filename.size(); i++){
            if(filename[i] == file){
                cout << "File " << file << " already exists." << endl;
                return 1;
            }
        }

        for(int i=0; i<filename.size(); i++){
            if (filename[i] == "XXXXX"){
                cout << "Creating file " << file << endl;
                filename[i] = file;
		firstblock[i]=0;
                break;
            }
        }
     	fssynch();
     	return 1;
}

int Filesys::rmfile(string file){
        for(int i = 0; i < filename.size(); i++) {
		if(filename[i] == file && firstblock[i] == 0) {
			filename[i] = "XXXXXX";
			firstblock[i] = 0;
		}
	}
        fssynch();
        return 1;
}

int Filesys::getfirstblock(string file){
        int first_block = 0;
        for(int i=0; i<filename.size(); i++){
                if(filename[i]==file){
                        first_block = firstblock[i];
                        break;
                }
        }
        fssynch();
        return first_block;
}

int Filesys::addblock(string file, string buffer){
	int firstblock = getfirstblock(file), newblock = fat[0];
	if(newblock == 0) {
		cout << "Disk is full!" << endl;
		return -1;
	}
	if(firstblock == 0) {//file is empty
		for(int i = 0; i < filename.size(); i++) {
			if(filename[i] == file) {
				firstblock = newblock;
				fat[0] = fat[newblock];
				fat[newblock] = 0;
			}
		}
	}
	else {
		int nextblock = firstblock;
		while(fat[newblock] != 0) {
			nextblock = fat[nextblock];
			fat[nextblock] = nextblock;
			fat[0] = fat[newblock];
			fat[newblock] = 0;
		}
	}
	putblock(newblock, buffer);
	fssynch();
	return newblock;
}

int Filesys::delblock(string file, int blocknumber){
	if(blocknumber == getfirstblock(file)) {
		for(int i = 0; i < filename.size(); i++) {
			if(filename[i] == file) {
				firstblock[i] = fat[blocknumber];
			}
		}
	}
	else {
		int blockid = getfirstblock(file);
		while(fat[blockid] != blocknumber) {
			blockid = fat[blockid];
		}
		fat[blockid] = fat[blocknumber];
	}
	fat[blocknumber] = fat[0];
	fat[0] = blocknumber;
	fssynch();
}

vector<string> Filesys::ls(){
	vector<string> flist;
	for(int i=0;i<filename.size();i++){
		if(filename[i]!="XXXXX"){
			flist.push_back(filename[i]);
		}
	}
	return flist;
}

int Filesys::readblock(string file, int blocknumber, string& buffer){
	getblock(blocknumber,buffer);
	return 1;
}
int Filesys::writeblock(string file, int blocknumber, string buffer){
	putblock(blocknumber,buffer);
	return 1;
}

int Filesys::nextblock(string file, int blocknumber){
	int blockid = getfirstblock(file);
	while(blockid != blocknumber){
		blockid = fat[blockid];
	}
	return fat[blockid];
}
