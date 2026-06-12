#include "RecordManager.h"
#include <fstream>
#include <iostream>

RecordManager::RecordManager(std::string fileName)
    : fileName(fileName)
{
}

void RecordManager::saveResult(std::string playerName, float gameTime, int score)
{
    std::fstream file;
    file.open(fileName, std::ios::out | std::ios::app);
    int minutes = static_cast<int>(gameTime) / 60;
    int seconds = static_cast<int>(gameTime) % 60;
    file << playerName << ";" << minutes << ";" << seconds << score << std::endl;

    file.close();
}

void RecordManager::setData() {
    std::fstream file;
    file.open(fileName, std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Nie mozna odczytac pliku";
    }
    std::string line;
    while (std::getline(file, line)) {
        data.push_back(line);
    }
    file.close();
}

std::vector<std::string> RecordManager::getData() {
    return data;
}