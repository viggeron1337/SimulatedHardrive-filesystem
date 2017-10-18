#include "filesystem.h"

const int MAXCOMMANDS = 8;
const int NUMAVAILABLECOMMANDS = 15;
FileSystem* fileSystem;

std::string availableCommands[NUMAVAILABLECOMMANDS] = {
    "quit","format","ls","create","cat","createImage","restoreImage",
    "rm","cp","append","mv","mkdir","cd","pwd","help"
};

/* Takes usercommand from input and returns number of commands, commands are stored in strArr[] */
int parseCommandString(const std::string &userCommand, std::string strArr[]);
int findCommand(std::string &command);
bool quit(); 
std::string help();
void format(); 
void listDirectory();
void append();
void create(std::string filePath); 
void mkdir(std::string filePath); 
int changeDirectory(std::string fileName, std::string& currentFolderName); 
void pwd(const std::vector<std::string>& pwdVector); 
int cat(std::string filePath); 
void rm(std::string filePath);
void createImage(std::string imageName);
void restoreImage(std::string imageName);
void cp(std::string originalName, std::string copisName);
/* More functions ... */

int main(void) {

	std::string userCommand, commandArr[MAXCOMMANDS];
	std::string user = "user@DV1492";    // Change this if you want another user to be displayed
	std::string currentDir = "/";    // current directory, used for output
	std::string filePath = ""; 
	std::vector<std::string> pwdVetor; 
	std::string currentFolderName = "";
	bool started = false;
    bool bRun = true;

    do {
        std::cout << user << ":" << currentDir << "$ ";
        getline(std::cin, userCommand);

        int nrOfCommands = parseCommandString(userCommand, commandArr);
        if (nrOfCommands > 0) {

            int cIndex = findCommand(commandArr[0]);
            switch(cIndex) {

			case 0: //quit
				bRun = quit();                
                break;
            case 1: // format
				format(); 
				currentDir = "/.";
				pwdVetor.clear();
				started = true;
                break;
            case 2: // ls
				if (started == true)
				{
					std::cout << "\n" << "Listing directory" << std::endl << std::endl; 
					std::cout << "Name:		Type:" << std::endl << std::endl; 
					listDirectory();
				}
                break;
            case 3: // create
				if (started == true)
				{
					create(commandArr[1]);
				}
                break;
            case 4: // cat
				if (started == true)
				{
					if (cat(commandArr[1]) == -1)
					{
						std::cout << "Error: File does not exist." << std::endl; 
					}
				}
                break;
            case 5: // createImage
				if (started == true)
				{
					createImage(commandArr[1]);
				}
                break;
            case 6: // restoreImage
				started = true;
				currentDir = "/.";
				pwdVetor.clear();
				restoreImage(commandArr[1]);
                break;
            case 7: // rm
				if (started == true)
				{
					rm(commandArr[1]);
				}
                break;
            case 8: // cp
				if (started == true)
				{
					cp(commandArr[1], commandArr[2]);
				}
                break;
            case 9: // append
				if (started == true)
				{
					append();
				}
                break;
            case 10: // mv
                break;
            case 11: // mkdir
				if (started == true)
				{
					mkdir(commandArr[1]);
				}
                break;
            case 12: // cd
				if (started == true)
				{
					if (changeDirectory(commandArr[1], currentFolderName) != -1)
					{
						pwdVetor.push_back(currentFolderName);
						currentDir += pwdVetor.back();
					}
					else
					{
						std::cout << "Error: folder does not exist." << std::endl;
					}
				}
                break;
            case 13: // pwd
				if (started == true)
				{
					pwd(pwdVetor);
				}
                break;
            case 14: // help
                std::cout << help() << std::endl;
                break;
            default:
                std::cout << "Unknown command: " << commandArr[0] << std::endl;
            }
        }
    } while (bRun == true);
    return 0;
}

int parseCommandString(const std::string &userCommand, std::string strArr[]) {
    std::stringstream ssin(userCommand);
    int counter = 0;
    while (ssin.good() && counter < MAXCOMMANDS) {
        ssin >> strArr[counter];
        counter++;
    }
    if (strArr[0] == "") {
        counter = 0;
    }
    return counter;
}
int findCommand(std::string &command) {
    int index = -1;
    for (int i = 0; i < NUMAVAILABLECOMMANDS && index == -1; ++i) {
        if (command == availableCommands[i]) {
            index = i;
        }
    }
    return index;
}

bool quit() {
	std::cout << "Exiting\n";
	return false;
}

std::string help() {
    std::string helpStr;
    helpStr += "OSD Disk Tool .oO Help Screen Oo.\n";
    helpStr += "-----------------------------------------------------------------------------------\n" ;
    helpStr += "* quit:                             Quit OSD Disk Tool\n";
    helpStr += "* format;                           Formats disk\n";
    helpStr += "* ls     <path>:                    Lists contents of <path>.\n";
    helpStr += "* create <path>:                    Creates a file and stores contents in <path>\n";
    helpStr += "* cat    <path>:                    Dumps contents of <file>.\n";
    helpStr += "* createImage  <real-file>:         Saves disk to <real-file>\n";
    helpStr += "* restoreImage <real-file>:         Reads <real-file> onto disk\n";
    helpStr += "* rm     <file>:                    Removes <file>\n";
    helpStr += "* cp     <source> <destination>:    Copy <source> to <destination>\n";
    helpStr += "* append <source> <destination>:    Appends contents of <source> to <destination>\n";
    helpStr += "* mv     <old-file> <new-file>:     Renames <old-file> to <new-file>\n";
    helpStr += "* mkdir  <directory>:               Creates a new directory called <directory>\n";
    helpStr += "* cd     <directory>:               Changes current working directory to <directory>\n";
    helpStr += "* pwd:                              Get current working directory\n";
    helpStr += "* help:                             Prints this help screen\n";
    return helpStr;
}

/* Insert code for your shell functions and call them from the switch-case */
void format()
{
	fileSystem = new FileSystem(); 
}

void listDirectory()
{
	std::string output = fileSystem->listDirectory();
	std::cout << output << std::endl;
}

void append()
{

} 

void create(std::string filePath)
{
	//filepath is in commandArr[1] which is passed through in filePath. 
	//The last section of that string is the name, contain it somehow.
	std::string fileContent = ""; 
	std::string name = filePath; 
	std::cout << "Please enter content of file: " << std::endl; 
	std::getline(std::cin, fileContent); 
	//Create file here 
	fileSystem->createFile(name, fileContent);
}

void mkdir(std::string filePath)
{
	//filepath is in commandArr[1] which is passed through in filePath. 
	//The last section of that string is the name, contain it somehow.
	std::string name = filePath; 
	/*Call filesystem makedir here*/
	//get Name
	fileSystem->createFolder(name);
}
int changeDirectory(std::string fileName, std::string& currentFolderName)
{
	return fileSystem->goToFolder(fileName, currentFolderName);
}
void pwd(const std::vector<std::string>& pwdVector) 
{
	for (int i = 0; i < pwdVector.size(); i++)
	{
		std::cout << pwdVector.at(i); 
	}
	std::cout << std::endl; 
}

int cat(std::string filePath)
{
	int found = -1; 
	std::string fileContent = ""; 
	found = fileSystem->cat(filePath,fileContent);
	if (found != -1)
	{
		std::cout << fileContent; 
	}
	return found; 
}

void rm(std::string filePath)
{
	fileSystem->remove(filePath);
}

void createImage(std::string imageName)
{
	if (imageName.size() <= 4 || imageName.substr(imageName.size() - 4, 4) != ".txt")
	{
		imageName += ".txt";
	}
	fileSystem->createImage(imageName);
}

void restoreImage(std::string imageName)
{
	fileSystem = new FileSystem(imageName);
}

void cp(std::string originalName, std::string copisName)
{
	fileSystem->cp(originalName, copisName);
}