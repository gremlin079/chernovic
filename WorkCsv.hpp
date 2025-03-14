#ifndef WORKCSV_HPP
#define WORKCSV_HPP
#include <fstream>
#include <string>
#include <map>
#include <vector>

//
struct Student
{
    std::string name;
    std::map<std::string, short> marks;
};

class Seminar
{
public:
    std::string name;
    std::vector<Student> students;
    std::vector<std::string> dates;
public:
    Seminar(std::string);
};



class CSVHandler
{
private:
    std::string path;
    std::vector<Seminar> seminars;
public:

    CSVHandler(std::string);
    std::vector<Seminar> getSeminars();
    void readData();
    void writeData(const std::vector<Seminar>&);

};
#endif