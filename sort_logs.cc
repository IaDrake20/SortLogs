/**
 * Ian Drake
 * 8/30/2022
 * simpler form of log_file_processing
 * The program will find files called mgwd.log. and vifmgr.log. and build them into a text document. 
 * It will then read through the text doc and sort them according to date.
 * Logs will be grouped with other logs from the same computer, computers will have their logs seperated
**/
//#include <ostream>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <vector>
#include <regex>
#include <tuple>
#include "DateTime.h"

namespace fs = std::experimental::filesystem;
class logline {

    public:
        DateTime dt;
        std::string node;
        std::string module;
        std::string logtext;

        bool operator < (const logline& rhs){
            return dt < rhs.dt;
        } 

        std::string to_string() {
            return dt.to_string() + " " +node +" " +module+" "+logtext;
            //work with module + log_txt some way with |
            //iterate array call to_string
            //make sure that garbage doesn't get through
        }
};



std::tuple <logline, std::string, std::string, bool> filter_string(std::string line, char **argv){
    std::string filt = "(...)\\s+(\\d\\d)\\s+(\\d\\d\\d\\d)\\s+(\\d\\d):(\\d\\d):(\\d\\d)";
    std::smatch m;
    bool success = false;
    logline ll;

    std::string date = line.substr(26, 26);
    std::string comp_logger = line.substr(50, 31);

    //iterate to node from filepath 
    std::string path = argv[1];
    //std::string p_node = path.substr(path.find_last_of("/"));

    //std::cout << "before filter check" << std::endl;
    //std::cout << "\nCurrent line is " << line << std::endl;
    bool x = std::regex_search(date, m, std::regex(filt));
    if(x){
        success = true;
        ll.node = path.substr(path.find_last_of("/"));;
        ll.module = comp_logger;
        ll.logtext = line.substr(81, 40);//40 is made up, need specific #
        //setup node, module, log_text

    } else {
        std::cout << date << " did not pass the filter." << std::endl;
    }
    return {ll, date, comp_logger, success};
}


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


    std::cout << "File is open? " <<fileHandle.is_open() << std::endl;
    for(int i = 0; i < content.size(); i++){
        std::string con = content.at(i);
        std::cout << " i is " << i << std::endl;
        fileHandle << content.at(i) << std::endl;
        fileHandle << i << std::endl;

        std::cout << con << std::endl;
        fileHandle << con << std::endl;
        
    }
    fileHandle.close();
}

//append instead of overwrite to file
void appendWriteFile(std::string path, std::string name, std::vector<std::string> &content){
    std::string fileName;
    fileName = path +"/"+ name;
    std::ofstream fileHandle(fileName);
    

}

//return vector of what files are found
std::vector<std::string> findFiles(std::string rootDir, std::string pattern)
{   
    std::vector<std::string> returnMe;
    std::cout << "looking for " << pattern << std::endl;
    for (auto entry: fs::recursive_directory_iterator(rootDir)){
        //use slashes to find node name after pulling argv 1 as param
        std::string fname = entry.path();
        if (fname.find(pattern) != std::string::npos){
            std::cout << entry << std::endl;
            returnMe.push_back(entry.path());
        }
    }
    return returnMe;
} 

void readFile (std::string path, std::vector<std::string>& destination, char **argv)
{
    std::string fileName = path;
    std::ifstream fileHandle(fileName);
    std::string line;

    while (std::getline(fileHandle, line)){
        auto [a,b,c,d] =  filter_string(line, argv);
        std::cout << "logline..." << a.node << " " << a.module << std::endl;
        destination.push_back(line);
    }
    fileHandle.close();
}

void printFile (std::string path){
    std::string fileName = path;
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

//returns a sorted txt of the files listed in the vector. It is assumed that all the files are from the same computer that reported
void sortFilesFromRaw (std::vector<std::string> files, std::string fileName, char **argv){
    std::vector<std::string> fileLines;
    std::vector<logline> log_line;
    DateTime dt;



    std::string filter = "(...)\\s+(\\d\\d)\\s+(\\d\\d\\d\\d)\\s+(\\d\\d):(\\d\\d):(\\d\\d)";

    //change to use it to start from front
    while(files.size() > 0){
        std::string f = files.back();
        readFile(f, fileLines, argv);
        files.pop_back();
    }

    std::vector<std::string>::iterator it;
    //int i = 0;
    for(it = fileLines.begin(); it != fileLines.end(); it++){

        auto [line_date, line_complog, line_combined, success] = filter_string(*it, argv);

        if(success){
            //replace with 
            *it == line_combined;

        } else {
        }
        //fileLines.erase(it);
    }


    //std::cout << "Write iteration... " << i <<" ..............." << fileLines.back() << std::endl;
    writeFile("/home/ian/CPprograms/c++", fileName, fileLines);

    for(it = fileLines.begin(); it != fileLines.end(); it++){

    }

    std::cout << "after filter" << std::endl;
    std::cout << "creating file now after filter." << std::endl;
    //createFile("/home/ian/C programs/SortDateProject", fileName);
}

//expects root user to search, string to name file (txt auto included), path to where to write file, what to look for
int main(int argc, char **argv){
    std::string user_root = argv[0];

    std::string fileName = argv[2];
    fileName += ".txt";
    std::vector<std::string> vifmgr_files;
    std::vector<std::string> mgwd_files;

    std::vector<std::string> files = findFiles(argv[1], argv[4]);//"/dan/home/C projects/", "mgwd.log.");//argv[0], "mgwd.log.");

    createFile(argv[3], fileName);

    //takes the vector's list of files and returns a name of a file that holds the sorted dates
    //sortFilesFromRaw(vifmgr_files, "vifs.txt");
    //sortFilesFromRaw(mgwd_files, "mgwds.txt");
    sortFilesFromRaw(files, fileName, argv);
    return 0;
}
