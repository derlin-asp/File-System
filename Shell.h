//
// Created by dave on 5/10/19.
//

#ifndef FILE_SYSTEM_SHELL_H
#define FILE_SYSTEM_SHELL_H


class Shell:public Filesys{
public:
    Shell(string diskname,int blocksize, int numberofblocks);
    int dir();
    int add(string file);
    int del(string file);
    int type(string file);
    int copy(string file1,string file2);
};


#endif //FILE_SYSTEM_SHELL_H
