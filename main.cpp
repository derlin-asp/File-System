//
// Created by dave on 5/10/19.
//
#include "Sdisk.h"
#include <iostream>
using namespace std;



int main()
{

    Sdisk disk1("disk1", 128,128);
   // Filesys fsys(disk1);

/*


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

    */


}