#ifndef WORKCSV_HPP
#define WORKSCV_HPP
#include <fstream>
#include <string>
#include <map>
#include <vector>

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


void FileToStudent(std::string);


#endif

class CSVHandler
{
private:
    std::string path;
    std::vector<Seminar> seminars;
public:

    CSVHandler(std::string);
    void addSeminar(const Seminar&);
    void updateSeminar(const Seminar&);
    void readData();
    void saveData();
    std::vector<Seminar> getSeminars();
};