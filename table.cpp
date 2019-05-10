//
// Created by dave on 5/10/19.
//



Table::Table(string filename,int numberofblocks,int blocksize,string flatfile,string indexfile):Filesys(filename,numberofblocks,blocksize){
    this->indexfile = indexfile;
    this->flatfile = flatfile;
    newfile(flatfile);
    newfile(indexfile);
}

int Table::Build_Table(string input_file){
    ifstream infile;
    infile.open(input_file.c_str());
    vector<string> tKey;
    vector<int> tBlock;
    string record;
    getline(infile, record);
    int number = 0;
    while(infile.good()){
        string primeKey = record.substr(0,5);
        vector<string> blocks = block(record,getblocksize());
        int blockid = addblock(flatfile,blocks[0]);
        tKey.push_back(primeKey);
        tBlock.push_back(blockid);
        getline(infile,record);
        if(infile.bad() and tKey.size()>0){
            ostringstream tBuffer;
            tBuffer<<tKey.size()<<" ";
            for(int i=0;i<tKey.size();i++){
                tBuffer<<tKey[i]<<" "<<tBlock[i]<<" ";
            }
            string buffer = tBuffer.str();
            vector<string> blocks2 = block(buffer,getblocksize());
            int error = addblock(indexfile,blocks2[0]);
            tKey.clear();
            tBlock.clear();
        }
    }
}

int Table::IndexSearch(string key){
    int block = getfirstblock(indexfile);
    while(block!=0){
        string buffer;
        readblock(indexfile,block,buffer);
        istringstream istream;
        istream.str(buffer);
        int number = 0;
        istream>>number;
        for(int i=0;i<number;i++){
            string s;
            int b;
            istream>>s>>b;
            if(key==s){
                return b;
            }
        }
        block = nextblock(indexfile,block);
    }
}

int Table::Search(string value){
    int block = IndexSearch(value);
    if(block==0){
        cout<<"No Record"<<endl;
    }
    else{
        string buffer;
        readblock(flatfile,block,buffer);
        cout<<buffer<<endl;
    }
}


