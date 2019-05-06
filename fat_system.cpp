#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;
/*
File System
*/



vector<string> block(string buffer, int b);



/*
Sdisk Class
Base Class
Creates the "Hard Disk using a text file"

*/


class Sdisk
{
    
public :
    Sdisk(string diskname, int numberofblocks, int blocksize); 
    Sdisk();
    int getblock(int blocknumber, string& buffer);      //retrns   
    int putblock(int blocknumber, string buffer);
    int getnumberofblocks();                        // accessor function
    int getblocksize();                             // accessor function
private :
    string diskname;                             // file name of software-disk
    int numberofblocks;                           // number of blocks on disk
    int blocksize;                                // block size in bytes
};


class Filesys: public Sdisk
{
public :
    Filesys();
    Filesys(Sdisk& disk);
    bool findfile(string, int*);
    int fsclose();
    int fssynch(Sdisk& disk);
    int newfile(string file, Sdisk&);
    int rmfile(string file, Sdisk&);
    int getfirstblock(string file);
    int addblock(string file, string block,Sdisk& hdisk);
    int delblock(string file, int blocknumber, Sdisk& hdisk);
    int readblock(string file, int blocknumber, string& buffer);
    int writeblock(string file, int blocknumber, string buffer);
    int nextblock(string file, int blocknumber);
private :
    
    int rootsize;           // maximum number of entries in ROOT
    int fatsize;            // number of blocks occupied by FAT
    vector<string> filename;   // filenames in ROOT
    vector<int> firstblock; // firstblocks in ROOT
    vector<int> fat;             // FAT
};


int main()
{
 
    Sdisk disk1("disk1", 128,128);
    Filesys fsys(disk1);
    
    
    
 
   fsys.newfile("file1", disk1); //file 1
   fsys.newfile("file2",disk1);  //file 2

   string bfile1;
   string bfile2;

  for (int i=1; i<=1024; i++) //1024 is number of bytes
     {
       bfile1+="1";
     }

  vector<string> blocks=block(bfile1,128); 

  int blocknumber=0;

  for (int i=0; i< blocks.size(); i++)
     {
       blocknumber=fsys.addblock("file1",blocks[i], disk1);
     }
    
  fsys.delblock("file1",fsys.getfirstblock("file1"), disk1);

  for (int i=1; i<=2048; i++)
     {
       bfile2+="2";
     }

  blocks=block(bfile2,128); 

  for (int i=0; i< blocks.size(); i++)
     {
       blocknumber=fsys.addblock("file2",blocks[i], disk1);
     }

  fsys.delblock("file2",blocknumber, disk1);
    
    
    
   
}

//START OF FILESYS FOR LAB 4 ****************************
  bool Filesys::findfile( string file, int* index )
    {
    	for( int i = 0; i < filename.size( ); ++i )
    		if( filename[i] == file )
    		{
    			if( index ) *index = i;
    			return true;
    		}
        
    	return false;
    }




int Filesys::newfile( string file, Sdisk& hdisk)
    {
    	//if( findfile( file ) )
    	//	return 0; // File found.
        
    	if( file.size( ) > 5 )
    	{
    		cout << "Filename '" << file << "' too large, max size is " << 5 << " chars!" << endl;
    		return 0; // Filename too large.
    	}
        
    	for( int i = 0; i < filename.size( ); ++i )
    		if( filename[i] == "$$$$$" )
    		{
    			filename[i]   = file;
    		}
    		
    		fssynch(hdisk);
    		//for( int i = 0; i < filename.size( ); ++i )
    		  //cout << "  " << filename[i] << "  " << endl;
    }


//RemoveFile
    int Filesys::rmfile(string file, Sdisk& hdisk)
    {
    	int index;
    	if( findfile( file, &index ) )
    		if( firstblock[index] == 0 )
    		{
    			filename[index] = "$$$$$";
    			fssynch(hdisk);
    			return 1;
    		}
        
    	return 0; // File not empty or was not found.
    }

 //GetFirstBlock
    int Filesys::getfirstblock(string file)
    {
        int index;
    	if( findfile( file, &index ) )
    		return firstblock[index];
        
    	return 0; // File does not exist.
    }

   int Filesys::addblock( string file, string buffer,Sdisk& hdisk )
    {
    	if( fat[0] == 0 )
    		return -1; // File system full.
        
    	int index;
    	if( !findfile( file, &index ) )
    		return 0; // File not found.
        
    	// Update free list.
    	int ablock = fat[0];
    	fat[0] = fat[ablock];
        
    	// Get first block.
    	int fblock = firstblock[index];
    	
    	// If the file is empty, update first block list.
    	if( fblock == 0 ) 
    	{
    		firstblock[index] = ablock;
    		fat[ablock] = 0;
    	}
    	// Search through FAT for the last block in file.
    	else
    	{
    		while( fat[fblock] != 0 )
    			fblock = fat[fblock];
            
    		fat[fblock] = ablock;
    		fat[ablock] = 0;
    	}
        
    	hdisk.putblock( ablock, buffer );
        
    	fssynch(hdisk);
        
    	return ablock;
    }


  //DeleteBlock
    int Filesys::delblock(string file, int blocknumber, Sdisk& hdisk)
    {
        bool found = false;
        int block = getfirstblock(file);
        if (block == 0)
        {
            return 0;
        }
        if(block == blocknumber)
        {
            found = true;
            for (int i = 0; i < rootsize; i++)
            {
                if(filename[i] == file)
                    firstblock[i] = fat[block];
                break;
            }
        }
        else // blocknumber is not the first block
        {
            int i = block;
            while(fat[i] != 0)
            {
                if(fat[i] != 0)
                {
                    fat[i] == fat[fat[i]];
                    found == true;
                }
            }
        }
        if (!found)
        {
            return 0;
        }
        fat[blocknumber] = fat[0];
        fat[0] = blocknumber;
        fssynch(hdisk);
    }



//END OF LAB 4 EVERYTHING BELOW IS LAB 3 *************************************

Filesys::Filesys(Sdisk& hdisk) : Sdisk()   //passing Sdisk into Filesys COnstructor instead of the three parameters
{
    
    string buffer;
    hdisk.getblock(2, buffer); 
    rootsize = hdisk.getblocksize()/10; 
    
   
    fatsize = 4*hdisk.getnumberofblocks()/hdisk.getblocksize();  

     //cout << rootsize<< "    " << fatsize << endl;
    if(buffer[0] == '#') {
      //  cout << "TEST" << endl;
       // cout << "Building ROOT" << endl;
        for(int i = 0; i <  rootsize; i++ ) {    
            filename.push_back("$$$$$");
            firstblock.push_back(0);
        }
        
        //fat.push_back(2 + fatsize );   
        //cout << hdisk.getnumberofblocks() << endl;
        for(int i = 0; i < hdisk.getnumberofblocks(); i++) { // 1+ fatsize or getnumber of blocks
            fat.push_back(0);
        
        }
     
        fat[0] = fatsize + 2;

        for(int i = 2 + fatsize; i < hdisk.getnumberofblocks() -1; i++ ) //fatsize or getnumberofblocks -1
            fat[i] = i +1;
    
        fat[fat.size()-1] = 0; 
    
    }else{
      
            string root; 
            hdisk.getblock(1,root); 
            
            istringstream rstream(root);
            
            for(int i = 0; i < rootsize; ++i )
                rstream >> filename[i] >> firstblock[i];
            
            string Fbuffer;
            
            for(int i = 0; i < fatsize; ++i)
            {
                string a;
                hdisk.getblock(i + 2, a);
                Fbuffer += a;
            }
            
            istringstream FATstream(Fbuffer);
            
            for(int i = 0; i < hdisk.getnumberofblocks( ); ++i)
                FATstream >> fat[i];
    
    }
        fssynch(hdisk);
}


int Filesys::fssynch(Sdisk& hdisk )
{

    ostringstream rootstream;
    for(int i = 0; i < rootsize; ++i){
        rootstream << filename[i] << " " << firstblock[i] << " ";  
    }
    string root = rootstream.str();
    for(int j = root.size(); j < hdisk.getblocksize(); j++){
        root += "$";
    }
    putblock(1, root);
  
    ostringstream fatstream;
    
    for(int i = 0; i < hdisk.getnumberofblocks(); i++){
        fatstream << fat[i] << " ";
    }
    string fatstring = fatstream.str();
  
    for(int i = 0; i < hdisk.getblocksize(); i++){
        fatstring += "$";
    }
    
    
    
    
    for(int i = 0, blocks = 2; i < fatsize; i++){
        string temp = fatstring.substr(i * hdisk.getblocksize(), hdisk.getnumberofblocks());
        hdisk.putblock(i + blocks, temp);
    }
   
    return 1; 
}




//END OF LAB 3 EVERYTHING BELOW IS LAB 2 ******************************************


Sdisk::Sdisk() {} //temp constructor to start filesys just used to get rid of an error


Sdisk::Sdisk(string disk_name, int numblocks, int block_size)
{
    blocksize = block_size;
    numberofblocks = numblocks;
    ifstream ifs(disk_name);
    diskname = disk_name;
    
    ifs.seekg(0,ios_base::end);
    ios_base::streampos end_pos = ifs.tellg();
    
    
    
    if( ifs.is_open() ) {  
       if(end_pos == numblocks*block_size)
            cout << "Disk Exists " << endl;
            
        
    }else{
        
        diskname = disk_name;
        ofstream ostr;
        ostr.open( disk_name.c_str() );
        for(int i = 0; i < (numblocks*block_size); i++) {
            ostr << "#";
        }
        ostr.close();
    }
}   

//writes the string buffer to block blocknumber. It returns an error code of 1 if successful and 0 otherwise.
int Sdisk::putblock(int num_block, string buffer) {
        
        fstream iofile;
        iofile.open(diskname.c_str(),ios_base::in | ios_base::out );
        iofile.seekp(num_block*blocksize);
        for(int i = 0; i < /*/num_block**/blocksize; i++)
            iofile << buffer[i];
}

//retrieves block blocknumber from the disk and stores the data in the string buffer. It returns an error code of 1 if successful and 0 otherwise.
int Sdisk::getblock(int num_block, string& buffer) {
        //cout << "INSIDE GETBLOCK   ";
        //cout << "       " << blocksize;
        buffer = "";
        fstream iofile;
        iofile.open(diskname.c_str(),ios_base::in | ios_base::out );
        iofile.seekp(num_block*blocksize, ios::beg);
        
        for(int i = 0; i < /*num_block*/blocksize; i++){
            //iofile >> buffer; 
            char a = iofile.get();
            //iofile.put(buffer[i]);
            buffer += a;
        }
        iofile.close();
}   


int Sdisk::getblocksize() {
    return blocksize;
}


int Sdisk::getnumberofblocks() {
    return numberofblocks;       
}


vector<string> block(string buffer, int b)
{
// blocks the buffer into a list of blocks of size b

vector<string> blocks;
int numberofblocks=0;

if (buffer.length() % b == 0) 
   { numberofblocks= buffer.length()/b;
   }
else 
   { numberofblocks= buffer.length()/b +1;
   }

string tempblock;
for (int i=0; i<numberofblocks; i++)
    { tempblock=buffer.substr(b*i,b);
      blocks.push_back(tempblock);
    }

int lastblock=blocks.size()-1;

for (int i=blocks[lastblock].length(); i<b; i++)
    { blocks[lastblock]+="#";
    }

return blocks;

}
