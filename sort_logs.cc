/**
 * Ian Drake
 * 8/30/2022
 * simpler form of log_file_processing
 * The program will find files called mgwd.log. and vifmgr.log. and build them into a text document. 
 * It will then read through the text doc and sort them according to date.
 * Logs will be grouped with other logs from the same computer, computers will have their logs seperated
**/

#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <vector>
#include <regex>
#include "DateTime.h"

namespace fs = std::experimental::filesystem;


//create empty file within specified dir
void createFile( std::string path, std::string name){
    std::cout << "creating " << name << std::endl;
    std::ofstream outputFile;

    outputFile.open(path + "/" + name, std::ios::out);
    outputFile.close();

}

void writeFile(std::string path, std::string name, std::vector<std::string>& content)
{   
    std::string fileName; 
    fileName = path + "/" + name;
    std::ofstream fileHandle(fileName);

    std::cout << "Size is " << content.size() << " and the name of the data is " << fileName << std::endl;

    fileHandle.open(fileName, std::ios::app);
    for(int i = 0; i < content.size(); i++){
        std::string con = content.at(i);
        //std::cout << " i is " << i << std::endl;
        //fileHandle << content.at(i) << std::endl;
        fileHandle << i << std::endl;
        
    }
    fileHandle.close();
}

//return vector of what files are found
std::vector<std::string> findFiles(std::string rootDir, std::string pattern)
{   
    std::vector<std::string> returnMe;
    std::cout << "looking for " << pattern << std::endl;
    for (auto entry: fs::recursive_directory_iterator(rootDir)){
        std::string fname = entry.path();
        if (fname.find(pattern) != std::string::npos){
            //std::cout << entry << std::endl;
            returnMe.push_back(entry.path());
        }
    }
    return returnMe;
} 

//read from file, plug into DateTime, work with 
void readFile (std::string path, std::vector<std::string>& destination)
{
    std::string fileName = path;
    std::ifstream fileHandle(fileName);
    std::string line;
    while (std::getline(fileHandle, line)){
        //std::cout << line << std::endl;
        destination.push_back(line);
    }
    fileHandle.close();
}

void deleteFile(std::string path) {
    std::cout << "deleting " << path << std::endl;
    fs::path p(path);
    if (fs::remove(p)){
        std::cout << path << " was deleted" << std::endl;
    }
    else {
        std::cout << "failed to delete " << path << std::endl;
    }
} 

//returns a sorted txt of the files listed in the vector. It is assumed that all the files are from the same computer that reported
void sortFilesFromRaw (std::vector<std::string> files, std::string fileName){
    std::vector<std::string> fileLines;
    DateTime dt;

    std::string filter = "(...)\\s+(\\d\\d)\\s+(\\d\\d\\d\\d)\\s+(\\d\\d):(\\d\\d):(\\d\\d)";

    //see what is contained in files
    std::cout << "file list within sortFilesFromRaw is size.. " << files.size() << "and contains..." << std::endl;
    //for(int i = 0; i < files.size(); i ++){
      //  std::cout << ".............." << files.back();
      //  files.pop_back();
    //}


    while(files.size() > 0){
        std::string f = files.back();//access an element
        readFile(f, fileLines);

        std::cout << "size of fileLines is..." << fileLines.size() << std::endl;
        //redun
        //std::cout << "pushing " << rippedLines.back() << "to fileLines which now contains " << fileLines.back() << std::endl;
        //std::cout << "filelines.back is ..." << fileLines.back() << std::endl;

        files.pop_back();
    }

        for(int i = 0; i < fileLines.size(); i++){
            //std::cout << "current iteration is..................................... " << i << std::endl;
            std::smatch m;
            bool x = std::regex_search(fileLines.at(i), m, std::regex(filter));

            if(!x){
                std::cout << "line... " << fileLines.at(i) << " doesn't match." << std::endl;
               // fileLines.erase(i+1);
            }
        }

    //test that fileLines has content to write to files
    //std::vector<std::string> copy(fileLines);
    //std::cout << "size of filelines is " << fileLines.size() << std::endl;
    //for(int i = 0; i < fileLines.size(); i++){
      //  std::cout << i << ":::::::::::::::::::::::::::::;......" << fileLines.at(i) << std::endl;;
    //}

    std::cout << "creating file now after filter." << std::endl;
    //createFile("/home/ian/C programs/SortDateProject", fileName);

    std::cout << "Size of the file before writing is..." << fileLines.size() << std::endl;

    //std::cout << "Write iteration... " << i <<" ..............." << fileLines.back() << std::endl;
    writeFile("/home/ian/C programs/SortDateProject", fileName, fileLines);

    fileLines.clear();

    //sort within file using dt
}

//combine files into a single file sorted by computer groups, delete the individual files for memory
//void sortFilesFromGroup()

int main(int argc, char **argv){

    //in future use path of current file to have a broad search for where logs are stored. Once they are found paths can be inserted

    //find some way to automate this part instead of specifying manually
    std::vector<std::string> vifmgr_files;
    std::vector<std::string> mgwd_files;

    vifmgr_files = findFiles("/home/dan/projects/c++/log_file_processing/asup","vifmgr.log.");
    mgwd_files = findFiles("/home/dan/projects/c++/log_file_processing/asup", "mgwd.log.");

    std::cout << "vigmgr files list is size..." << vifmgr_files.size() << std::endl;
    std::cout << "mgwd files list is size..." << mgwd_files.size() << std::endl;

    //takes the vector's list of files and returns a name of a file that holds the sorted dates
    sortFilesFromRaw(vifmgr_files, "vifs.txt");
    //sortFilesFromRaw(mgwd_files, "mgwds.txt");

//readd from the files to ensure they were written to
    //std::cout << "reading the files before deletion \n\n" << std::endl;
    //readFile("/home/ian/C programs/SortDateProject/vifs.txt");
    //readFile("home/ian/C programs/SortDateProject/mgwds.txt");

//for now delete, but in future 
    //deleteFile("/home/ian/C programs/SortDateProject/vifs.txt");
    //deleteFile("/home/ian/C programs/SortDateProject/mgwds.txt");

    return 0;
}