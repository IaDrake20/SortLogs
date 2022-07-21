#include <iostream>
#include <string>
#include <unordered_map>
#include <regex>
#include <cstdint>
#include <bits/stdc++.h>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <memory.h>

class DateTime
{
public:

    // convert from a string to a DateTime by using regular expressions
    bool convert_via_regex(std::string date_time)
    {
bool ret_val = false;

_date = date_time;

_time = 0;
memset(&_dt, 0, sizeof(_dt));

// mon day year hour min sec
const std::string pat = "(...)\\s+(\\d\\d)\\s+(\\d\\d\\d\\d)\\s+(\\d\\d):(\\d\\d):(\\d\\d)";
std::regex reg(pat);
std::smatch match;

if (std::regex_search(date_time, match, reg)){
   int8_t month;
   int8_t day;
   int16_t year;
   int8_t hour;
   int8_t min;
   int8_t sec;

   month = month_to_num(match[1].str().c_str());
   day = std::stol(match[2]);
   year = std::stol(match[3]);
   hour = std::stol(match[4]);
   min = std::stol(match[5]);
   sec = std::stol(match[6]);

   _dt.tm_sec = sec;
   _dt.tm_min = min;
   _dt.tm_hour = hour;
   _dt.tm_year = year - 1900;
   _dt.tm_mon = month - 1;
   _dt.tm_mday = day;

   _time = mktime(&_dt);
   
   ret_val = true;

}
else {
   std::cout << "invalid date time format: " << date_time << std::endl;
}

return ret_val;
    }

    // convert to DateTime using c++ library calls and format strings
    bool convert_via_string(std::string date_time)
    {
_time = 0;
_date = date_time;
memset(&_dt, 0, sizeof(_dt));

// need to check for errors here
(void)strptime(date_time.c_str(), "%b %d %Y %H:%M:%S", &_dt);
_time = mktime(&_dt);
return true;
    }

    // this needs to be defined to allow us to sort
    bool operator < (const DateTime& rhs){
std::string x = (_time < rhs._time) ? "true" : "false";
std::cout << "comparing " << _time << " < " << rhs._time << ":" << x << ":" << rhs._time - _time << std::endl;
return _time < rhs._time;
    }

    // getter for our generated value
    time_t val() { return _time; }

    const std::string to_string(){
std::string ret_val;
std::stringstream ss;
ss << _date;
ss << ":";
ss << _time;
std::string str = ss.str();

return str;
    }

private:
    // convert from the month as a string to the month as a number
    // instead of writing a switch statement of a bunch of if statements
    // we create a map that only get initialized once (enforced by the static keyword)
    // and look up the number using the name as the key. This is slow if we were calling
    // it a couple times, but much faster if calling it a couple million times.
    int month_to_num(const char *month )
    {
static std::unordered_map<std::string,int> mapping {
   {"Jan",1},
{"Feb",2},
{"Mar",3},
{"Apr",4},
{"May",5},
{"Jun",6},
{"Jul",7},
{"Aug",8},
{"Sep",9},
{"Oct",10},
{"Nov",11},
{"Dec",12}
};

return mapping[month];
    }


private:
    tm _dt;
    time_t _time;
    std::string _date;
};


// if compiling to an executable called dt test with this command line
// ./dt "Mar 01 2022 11:00:00" "Mar 01 2022 110:00:01"
int main(int argc, char **argv)
{
   
    const std::string date_string = argv[1];

    DateTime d1, d2;

    if (! d1.convert_via_string( argv[1] )){
std::cout << "failed to convert " << argv[1] << std::endl;
exit(1);
    }

    if (! d2.convert_via_string( argv[2])){
std::cout << "failed to convert " << argv[2] << std::endl;
exit(1);
    }

    if (d1 < d2){
std::cout << d1.to_string() << " is less than " << d2.to_string() << std::endl;
    }
    else {
std::cout << d1.to_string() << " is not less than " << d2.to_string() << std::endl;
    }

    // read in all date times from command line
    // convert them to DateTime
    // sort them
    // print them out in sorted order
    std::vector<DateTime> array;
    for(int i = 1; i < argc; ++i){
DateTime dt;
if (!dt.convert_via_string(argv[i])){
   std::cout << "failed to convert " << argv[i] << std::endl;
   exit(1);
}
array.push_back(dt);
std::cout << "pushed back " << dt.to_string() << std::endl;
    }

    std::sort(array.begin(), array.end());
    std::sort(array.begin(), array.end());

    std::cout << "sorted date and times: " << std::endl;
    for(auto x:array){
std::cout << x.to_string() << std::endl;
    }

    std::cout << "done" << std::endl;

    return 0;
}