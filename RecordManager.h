//
// Created by Wetson on 12.06.2026.
//

#ifndef SFML_PROJECT_RECORDMANAGER_H
#define SFML_PROJECT_RECORDMANAGER_H
#include <string>
#include <vector>
#include <string>
#include <iostream>
class RecordManager {
private:
    std::string fileName;
    std::vector<std::string> data;
public:
    RecordManager(std::string fileName);
    void saveResult(std::string playerName, float gameTime, int score);
    void setData();
    std::vector<std::string> getData();
};


#endif //SFML_PROJECT_RECORDMANAGER_H