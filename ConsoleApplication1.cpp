#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include "WorkCsv.hpp"
#include <sstream>
#include <filesystem>


Seminar::Seminar(std::string name)
{
    this->name = name;
}

CSVHandler::CSVHandler(std::string path)
{
    this->path = path;
    std::vector<Seminar> a = {};
    this->seminars = a;
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
            std::vector<std::string> dates;
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
            while (std::getline(file, line))
            {
                std::stringstream ss(line);
                std::string block;
                int counter2 = 0;
                Student newobj;
                std::map<std::string, std::string> MarksForRead;
                std::vector<std::string> HelpMarks;
                std::string NameForRead;
                while (std::getline(ss, block, ';'))
                {
                    if (counter == 0)
                    {
                        if (block[0] == -17)
                        {
                            continue;
                        }
                        else
                        {
                            dates.push_back(block);
                        }
                    }
                    else
                    {
                        if (counter2 == 0)
                        {
                            NameForRead = block;
                        }
                        else
                        {
                            HelpMarks.push_back(block);
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

            Seminar a(name);
            a.dates = dates;
            a.students = students;
            seminars.push_back(a);
            file.close();
        }
    }
    this->seminars = seminars;
}


int main()
{
    setlocale(LC_ALL, "Russian");
    CSVHandler a("C:\\Users\\Александр\\Documents\\жопа");
    a.readData();
    std::vector<Seminar> seminars = a.getSeminars();
    for (int i = 0; i < seminars.size(); i++)
    {
        std::cout << seminars[i].name << std::endl;
        for (int j = 0; j < seminars[i].students.size(); j++)
        {
            std::cout << seminars[i].students[j].name << std::endl;
            for (int k = 0; k < seminars[i].dates.size(); k++)
            {
                std::cout << seminars[i].dates[k] << " " << seminars[i].students[j].marks[seminars[i].dates[k]] << std::endl;
            }
        }
    }

    return 0;
}



