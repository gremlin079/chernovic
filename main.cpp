#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QDate>
#include <QString>
#include <QDir>

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include "WorkCsv.hpp"
#include <sstream>
#include <filesystem>
#include <cstdio>


Seminar::Seminar(QString seminarName)
{
    this->name = seminarName;
}

CSVHandler::CSVHandler(std::string path)
{
    this->path = path;
    this->seminars = {};
}

std::vector<Seminar> CSVHandler::getSeminars()
{
    return seminars;
}

void CSVHandler::readData()
{
    std::vector<Seminar> seminars;
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_regular_file())
        {
            std::ifstream file(entry.path());

            std::vector<QDate> dates;
            std::vector<Student> students;
            std::string line;
            int counter = 0;
            std::string name;
            int f = 0;
            std::string filename = entry.path().filename().string();
            while (filename[f] != '.')
            {
                name += filename[f];
                f += 1;
                if (f >= 50)
                {
                    break;
                }
            }

            std::cout << "Processing file: " << filename << std::endl;

            while (std::getline(file, line))
            {
                std::stringstream ss(line);
                std::string block;
                int counter2 = 0;
                Student newobj;
                std::map<QDate, short> MarksForRead;
                std::vector<short> HelpMarks;
                QString NameForRead;
                while (std::getline(ss, block, ';'))
                {
                    QString blockQString = QString::fromStdString(block);

                    if (counter == 0)
                    {
                        if (blockQString.startsWith("И"))
                        {
                            continue;
                        }
                        else
                        {
                            QDate date = QDate::fromString(blockQString, "dd-MM-yyyy");
                            if (date.isValid())
                            {
                                dates.push_back(date);
                                std::cout << "Date read: " << date.toString("dd-MM-yyyy").toStdString() << std::endl;
                            }
                        }
                    }
                    else
                    {
                        if (counter2 == 0)
                        {
                            NameForRead = QString::fromStdString(block);
                        }
                        else
                        {
                            if (blockQString.startsWith("Н"))
                            {
                                HelpMarks.push_back(-1);
                            }
                            else if (blockQString.startsWith("П"))
                            {
                                HelpMarks.push_back(0);
                            }
                            else if (blockQString.startsWith("Р"))
                            {
                                HelpMarks.push_back(1);
                            }
                        }
                    }
                    counter2 += 1;
                }
                if (counter != 0)
                {
                    newobj.name = NameForRead;
                    for (int i = 0; i < dates.size(); i++)
                    {
                        MarksForRead[dates[i]] = HelpMarks[i];
                    }
                    newobj.marks = MarksForRead;
                    students.push_back(newobj);
                }
                counter += 1;
            }

            Seminar a(QString::fromStdString(name));
            a.dates = dates;
            a.students = students;
            seminars.push_back(a);
            file.close();
        }
    }
    this->seminars = seminars;
}

void CSVHandler::writeData(const std::vector<Seminar>& a)
{
    int i = 0;
    std::vector<QDate> HelpDates = a[0].dates;

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_regular_file())
        {
            std::ofstream file(entry.path(), std::ios::trunc);

            std::string oldName = path + "/" + seminars[i].name.toStdString() + ".csv";
            std::string newName = path + "/" + a[i].name.toStdString() + ".csv";

            // Запись заголовка с датами
            file << "Имя студента;";
            for (const auto& date : HelpDates)
            {
                file << date.toString("dd-MM-yyyy").toStdString() << ";";
            }
            file << std::endl;

            std::cout << "Writing data for seminar: " << a[i].name.toStdString() << std::endl;

            // Запись данных студентов
            for (const auto& student : a[i].students)
            {
                file << student.name.toStdString() << ";"; // Имя студента

                std::cout << "Student: " << student.name.toStdString() << std::endl;

                for (const auto& date : HelpDates)
                {
                    if (student.marks.count(date))  // Если есть оценка для даты
                    {
                        short mark = student.marks.at(date);
                        if (mark == -1) file << "Н;";
                        else if (mark == 0) file << "П;";
                        else if (mark == 1) file << "Р;";
                    }
                    else
                    {
                        file << ";"; // Если оценки нет, пишем пустое значение
                    }
                }
                file << std::endl;
            }
            file.close();

            // Переименование файла с использованием стандартных функций C++
            std::string oldFile = oldName;
            std::string newFile = newName;

            if (std::rename(oldFile.c_str(), newFile.c_str()) != 0)
            {
                perror("Error renaming file");
                std::cerr << "Failed to rename file: " << oldFile << " to " << newFile << std::endl;
            }
        }
        i += 1;
    }
    this->seminars = a;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    CSVHandler a("C:\\testing"); // Убедитесь, что путь правильный
    a.readData();
    std::vector<Seminar> b = a.getSeminars();
    QString k = "test";
    b[0].name = k;
    a.writeData(b);

    return 0;
}


