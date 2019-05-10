//
// Created by dave on 5/10/19.
//


Shell::Shell(string diskname,int blocksize,int numberofblocks):Filesys(diskname,numberofblocks,blocksize){
}

int Shell::dir(){
    vector<string> flist=ls();
    for(int i=0;i<flist.size();i++){
        cout<<flist[i]<<endl;
    }
}

int Shell::del(string file){
    int block = getfirstblock(file);
    while(block > 0) {
        delblock(file, block);
        block = getfirstblock(file);
    }
    rmfile(file);
}

int Shell::copy(string file1, string file2){
    int block = getfirstblock(file1);
    int error = newfile(file2);
    string buffer;
    while(block > 0) {
        string t;
        error = readblock(file1, block, t);
        buffer += t;
        addblock(file2, buffer);
        block = nextblock(file1, block);
    }
}

int Shell::type(string file){
    int block = getfirstblock(file);
    string buffer;
    while(block > 0){
        string t;
        int error = readblock(file, block, t);
        buffer += t;
        block = nextblock(file, block);
    }
    cout << buffer << endl;
    cout << buffer.length() << endl;
}

int Shell::add(string file){
    newfile(file);
    string bfile;
    for(int i = 0; i < 128; i++) {
        bfile += "3";
    }
    vector<string> blocks = block(bfile, 128);


    for(int i = 0; i < blocks.size(); i++){
        addblock(file, blocks[i]);
    }
}
