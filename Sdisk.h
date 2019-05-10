//
// Created by dave on 5/9/19.
//

#ifndef FILE_SYSTEM_SDISK_H
#define FILE_SYSTEM_SDISK_H

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





#endif //FILE_SYSTEM_SDISK_H
