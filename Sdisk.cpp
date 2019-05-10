//
// Created by dave on 5/9/19.
//

#include "Sdisk.h"



vector<string> block(string buffer, int b);



Sdisk::Sdisk() {} //temp constructor to start filesys just used to get rid of an error


//Sdisk  init constructor checks for existing hard disk or makes one
Sdisk::Sdisk(string disk_name, int numblocks, int block_size)
{
    blocksize = block_size;
    numberofblocks = numblocks;
    ifstream ifs(disk_name);
    diskname = disk_name;

    ifs.seekg(0,ios_base::end);
    ios_base::streampos end_pos = ifs.tellg();


    //checks if file is open and whether it has data
    if( ifs.is_open() ) {
        if(end_pos == numblocks*block_size)
            cout << "Disk Exists " << endl;


    }else{ //otherwise creat a new hard disk

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
