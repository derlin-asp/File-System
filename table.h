//
// Created by dave on 5/10/19.
//

#ifndef FILE_SYSTEM_TABLE_H
#define FILE_SYSTEM_TABLE_H
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

#endif //FILE_SYSTEM_TABLE_H
