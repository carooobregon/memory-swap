#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "Command.h"

using namespace std;

Command processCmd(string line) {
    int space, nProc;
    bool modify;
    char type = line[0];
    Command cmd(type);
    space = line.find(" ");
    line = line.substr(space+1);
    switch (type) {
        case 'P': {
            space = line.find(" ");
            int nBytes = stoi(line.substr(0, space-1));
            line = line.substr(space+1);
            nProc = stoi(line);
            //TODO: try catch para error handling si stoi no jala, o no encuentra espacios
            cmd.setP(nBytes, nProc);
            break;
        }
        
        case 'A': {
            space = line.find(" ");
            int vDir = stoi(line.substr(0, space-1));
            line = line.substr(space+1);
            space = line.find(" ");
            nProc = stoi(line.substr(0, space-1));
            line = line.substr(space+1);
            if (stoi(line) == 1) {
                modify = true;
            } else {
                modify = false;
            }
            //TODO: try catch para error handling si stoi no jala, o no encuentra espacios
            cmd.setA(vDir, nProc, modify);
            break;
        }
        
        case 'L': {
            space = line.find(" ");
            nProc = stoi(line);
            //TODO: try catch para error handling si stoi no jala, o no encuentra espacios
            cmd.setL(nProc);
            break;
        }
            
        case 'E': {
            Command cmd(type);
            break;
        }
            
        case 'F': {
            Command cmd(type);
            break;
        }
            
        default:
            cout << "The command " << type << " " << line << "is invalid" << endl;
            exit(0);
            break;
    }
    
    return cmd;
}

vector<Command> readFile(string name) {
    vector<Command> cmds;
    string line;
    ifstream file;
    file.open(name);
    if(file.fail()) {
        cout << "There was a problem while opening the file" << endl;
        exit(0);
    } else {
        while (!file.eof()) {
            getline(file, line);
            Command temp = processCmd(line);
            cmds.push_back(temp);
        }
    }
    return cmds;
}

int main() {
    string name;
    vector<int> M;
    vector<int> S;
    vector<Command> cmds;

    cin >> name;
    cmds = readFile(name);
    return 0;

}

