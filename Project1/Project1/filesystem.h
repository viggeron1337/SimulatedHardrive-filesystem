#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"
#include <fstream>
#include <iostream>
#include <sstream>

class FileSystem
{
private:
    MemBlockDevice VHDD;
	char currentBlock;
	
	void format();

	void restoreImage(std::string imageName);

	/* Removes a file in the filesystem */
	void removeFile(char block);

	/* Removes a folder in the filesystem */
	void removeFolder(char block);
public:
    FileSystem();
	//This constructor calls on RestoreImage()
	FileSystem(std::string imageName);
    ~FileSystem();
	void append(std::string& appendInfo, char blockNr = '-1'); 
    /* These API functions need to be implemented
	   You are free to specify parameter lists and return values
    */

    /* This function creates a file in the filesystem */
    // createFile(...)
	void createFile(const std::string& strName, std::string& writeString);
	void createFile(std::string& writeString, int BlockNr);

    /* Creates a folder in the filesystem */
    // createFolder(...);
	void createFolder(const std::string &strName); 
	void createFolder(std::string& content, int BlockNr);

    /* Removes a file/folder in the filesystem */
	void remove(std::string name);

    /* Function will move the current location to a specified location in the filesystem */
    // goToFolder(...);
	int goToFolder(std::string folderName, std::string& resultName); 

    /* This function will get all the files and folders in the specified folder */

    // listDir(...);
	std::string listDirectory();

	/*Functon will return a string with the content of the specified file.*/
	int cat(std::string filePath, std::string& fileContent); 
	void cp(std::string originalName, std::string copisName);
    /* Add your own member-functions if needed */
	void createImage(std::string imageName);
};

#endif // FILESYSTEM_H
