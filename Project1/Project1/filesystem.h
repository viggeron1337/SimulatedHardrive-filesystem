#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"

class FileSystem
{
private:
    MemBlockDevice VHDD;
	int currentBlock;
	
	void format();
    // Here you can add your own data structures
public:
    FileSystem();
    ~FileSystem();
	std::string& listDirectory(); 
	void append(std::string& appendInfo, int blockNr = -1); 
    /* These API functions need to be implemented
	   You are free to specify parameter lists and return values
    */

    /* This function creates a file in the filesystem */
    // createFile(...)
	void createFile(const std::string& strName, std::string& writeString);

    /* Creates a folder in the filesystem */
    // createFolder(...);
	void createFolder(const std::string &strName); 

    /* Removes a file in the filesystem */
    // removeFile(...);

    /* Removes a folder in the filesystem */
    // removeFolder(...);

    /* Function will move the current location to a specified location in the filesystem */
    // goToFolder(...);

    /* This function will get all the files and folders in the specified folder */

    // listDir(...);

    /* Add your own member-functions if needed */
};

#endif // FILESYSTEM_H
