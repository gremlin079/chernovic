#ifndef WORKCSV_HPP
#define WORKCSV_HPP
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <QString>
#include <Qdate>

//
struct Student
{
    QString name;
    std::map<QDate, short> marks;
};

class Seminar
{
public:
    QString name;
    std::vector<Student> students;
    std::vector<QDate> dates;
public:
    Seminar(QString);
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
