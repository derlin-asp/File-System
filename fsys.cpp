#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;
/*
File System  - Code below is slightly incomplete more should be coming migrating stuff from Cloud 9 IDE.
Recreates a file system using a text file as a hard disk.

Uses different symbols to represent a used bit or block 
*/







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


