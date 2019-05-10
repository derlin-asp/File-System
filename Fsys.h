//
// Created by dave on 5/10/19.
//

#ifndef FILE_SYSTEM_FSYS_H
#define FILE_SYSTEM_FSYS_H



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









#endif //FILE_SYSTEM_FSYS_H
