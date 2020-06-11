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
            int nBytes = stoi(line.substr(0, space));
            line = line.substr(space+1);
            nProc = stoi(line);
            //TODO: try catch para error handling si stoi no jala, o no encuentra espacios
            cmd.setP(nBytes, nProc);
            break;
        }
        
        case 'A': {
            space = line.find(" ");
            int vDir = stoi(line.substr(0, space));
            line = line.substr(space+1);
            space = line.find(" ");
            nProc = stoi(line.substr(0, space));
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
            nProc = stoi(line);
            //TODO: try catch para error handling si stoi no jala, o no encuentra espacios
            cmd.setL(nProc);
            break;
        }

        case 'C': {
            Command cmd(type);
            cmd.setC(line);
            //TODO: try catch para error handling si stoi no jala, o no encuentra espacios
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

vector<Command> readFile() {
    vector<Command> cmds;
    string line;
    ifstream file;
    file.open("ArchivoTrabajo.txt");
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

void processP(){
    //WIP
}

void processA(){
    //WIP
}

void processL(){
    //WIP
}

void wipeMemory(){
    //WIP
}

int main() {
    string name;
    vector<int> M;
    vector<int> S;
    vector<Command> cmds;

    cmds = readFile();

    for (int i = 0; i < cmds.size(); i++) {
        switch (cmds[i].getType())
        {
        case 'P':
            processP();
            break;
        
        case 'A':
            processA();
            break;

        case 'L':
            processL();
            break;

        case 'C':
            cmds[i].print();
            break;

        case 'F':
            wipeMemory();
            break;

        case 'E':
            cout << "Bye! :) " << endl;
            exit(0);
            break;

        default:
            break;
        }
    }
    return 0;

}

