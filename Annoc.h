**********************
 * Ian Drake
 * Tuesday June 21st, 2022
 * Build a program that will annotate log files to be more useful
 *  1. Recognize log files
 *  2. store the file names in an array of strings
 *  3.
 * 
 **********************/

#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <vector>

namespace fs = std::experimental::filesystem;

std::string dirName;

std::vector<std::string> split(std::string& path) {

    std::vector<std::string> components;

    std::size_t end = 0;
    std::size_t start = 0;

    end = path.find("/", start);

    while(end != std::string::npos){
        std::string component = path.substr(start, end - start);
        components.push_back(component);
        start = end + 1 ;
        end = path.find("/", start);
    }

    // last component is remaining text
    components.push_back(path.substr(start, (path.size() - start)));


    return components;
}

void createDirectory(std::string name)
{
    std::string dirName{name};
    auto result = fs::create_directory(dirName);
    if (result){
        std::cout << "successfully create directory " << dirName << std::endl;
    }
    else {
        std::cout << "failed to create directory " << dirName << std::endl;
    }
}


//create dir as wanted, arg describes # of levels within...ISSUE with experimentql filesystem
std::string createDirectoryTree(int maxlevels, std::string dirname)
{
    std::string lvs;
    for(int i = 1; i <= maxlevels; i++){
        lvs.append("level" + std::to_string(i) + "/"); 
    }
    dirName = {dirname +"/" + lvs};
    auto result = fs::create_directories(dirName);
    if (result){
	    std::cout << "successfully create directory " << dirName << std::endl;
        return dirName;
    }
    else {
	    std::cout << "failed to create directory " << dirName << std::endl;
        return " ";
    }
}

//return vector of what is found
std::vector<std::string> findFiles(std::string rootDir, std::string pattern)
{   
    std::vector<std::string> returnMe;
    std::cout << "looking for " << pattern << std::endl;
    for (auto entry: fs::recursive_directory_iterator(rootDir)){
        std::string fname = entry.path();
        if (fname.find(pattern) != std::string::npos){
            std::cout << entry << std::endl;
            returnMe.push_back(entry.path());
        }
    }

    return returnMe;
} 


std::vector<std::string> createVector(int argc2, char **argv2){

    std::vector<std::string> names;

    if (argc2 < 2){
        std::cout << "usage: " << argv2[0] << " name1 name2 name..." << std::endl;
        return names;
    }

    for (int i = 0; i < argc2; ++i){
        names.push_back(argv2[i]);
    }

    return names;
}

//create empty file within specified dir
void createFile( std::string path, std::string name, std::vector<std::string>& lines)
{
    std::cout << "creating " << name << std::endl;
    std::ofstream outputFile;

    outputFile.open(path + "/" + name, std::ios::out);
    
    for(auto x = lines.begin(); x != lines.end(); ++x){
	    outputFile << *x;
    }
    outputFile.close();
}

void readFile (std::string path, std::string name)
{
    std::string fileName;
    fileName =  path + "/" + name;
    std::ifstream fileHandle(fileName);
    std::string line;
    while (std::getline(fileHandle, line)){
        std::cout << line << std::endl;
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

void deleteDirTree(std::string path) {
    std::cout << "deleting " << path << " and it's subdirectories" << std::endl;
    fs::path p(path);
    if (fs::remove_all(p)){
       std::cout << "deleted directory tree " << path << std::endl;
    }
    else {
        std::cout << "failed to delete directory tree " << path << std::endl;
    }
}

void writeFile(std::string path, std::string name)
{   
    std::string fileName; 
    fileName = path + "/" + name;
    std::ofstream fileHandle(fileName);
    for (int i = 5; i > 0; --i){ 
        fileHandle << "line " << i << std::endl;
    }
    fileHandle.close();
}