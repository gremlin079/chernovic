#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include "WorkCsv.hpp"
#include <sstream>
#include <filesystem>
#include <cstdio>
#include <locale>
#include <codecvt>

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
    //std::setlocale(LC_ALL, "ru_RU-UTF-8 BOM");
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
                std::map<std::string, short> MarksForRead;
                std::vector<short> HelpMarks;
                std::string NameForRead;
                while (std::getline(ss, block, ';'))
                {
                    if (counter == 0)
                    {
                        if (block[0] == 'И')
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
                            if (block[0] == 'Н')
                            {
                                HelpMarks.push_back(-1);
                            }
                            if (block[0] == 'П')
                            {
                                HelpMarks.push_back(0);
                            }
                            if (block[0] == 'Р')
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

            Seminar a(name);
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
    std::vector<std::string> HelpDates = a[0].dates;
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_regular_file())
        {
            std::ofstream file(entry.path(), std::ios::trunc);
            std::string oldName = path + "\\" + seminars[i].name + ".csv";
            std::string newName = path + "\\" + a[i].name + ".csv";
            
            
            /*rename(oldName.c_str(), newName.c_str());*/
            file << "Имя студента;";
            for (int j = 0; j < a[0].dates.size(); j++)
            {
                file << a[0].dates[j] << ";";
            }
            file << std::endl;
            for (int j = 0; j < a[i].students.size(); j++)
            {
                file << a[i].students[j].name << ";";
                for (int k = 0; k < HelpDates.size(); k++)
                {
                    if (a[i].students[j].marks.at(HelpDates[k]) == -1)
                    {
                        file << "Н;";
                    }
                    if (a[i].students[j].marks.at(HelpDates[k]) == 0)
                    {
                        file << "П;";
                    }
                    if (a[i].students[j].marks.at(HelpDates[k]) == 1)
                    {
                        file << "Р;";
                    }
                }
				file << std::endl;
            }
			file.close();
            if (rename(oldName.c_str(), newName.c_str()) != 0)
            {
                perror("Error renaming file");
            }
        
        }
        i += 1;
    }
    this->seminars = a;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    CSVHandler a("C:\\Users\\Александр\\Documents\\жопа");
	a.readData();
	std::vector<Seminar> b = a.getSeminars();
    b[0].name = "test";
    a.writeData(b);

    return 0;
}

