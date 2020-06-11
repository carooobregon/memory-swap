#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include "Command.h"

using namespace std;

vector<int> M;
vector<int> S;


Command processCmd(string line){
    char type = line[0];
    istringstream stream(line);
    Command cmd(type);
    string currWord;

    switch (type) {
        case 'P': {
            stream >> currWord;

            int nBytes, nProc;

            stream >> currWord;
            nBytes = stoi(currWord);
                
            stream >> currWord;
            nProc = stoi(currWord);
            cmd.setP(nBytes, nProc);
            break;
        }

        case 'A': {
            stream >> currWord;
            int vDir, nProc;
            bool modif;

            stream >> currWord;
            vDir = stoi(currWord);
                
            stream >> currWord;
            nProc = stoi(currWord);

            stream >> currWord;
            if(currWord == "1")
                modif = true;
            else
                modif = false;
                
            cmd.setA(vDir, nProc, modif);
            break;
        }

        case 'L':{
            stream >> currWord;
            int nProc;

            stream >> currWord;
            nProc = stoi(currWord);

            cmd.setL(nProc);

            break;
        }

        case 'C': {
            string output;
            while (stream >> currWord) {
                stream >> currWord;
                if (!output.empty()) // special case: no space before first word
                        output += ' ';
                    output += currWord;
            }
            cmd.setC(output);
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

void processL(int nProc){
    cout << "Liberar los marcos de pagina ocupados por el proceso " << nProc << endl;
    vector<int> freed;
    for (int i = 0; i < M.size(); i++) {
        if (M[i] == nProc) {
            M[i] = -1;
            freed.push_back(i);
        }
    }
    if (!freed.empty()) {
        cout << "Se liberan los marcos de memoria real: ";
        for(int i : freed) 
            cout << i << ", ";
        cout << endl;
    }
    freed.clear();
    for (int i = 0; i < S.size(); i++) {
        if (S[i] == nProc) {
            S[i] = -1;
            freed.push_back(i);
        }
    }
     if (!freed.empty()) {
        cout << "Se liberan los marcos [ ";
        for(int i : freed) 
            cout << i << ", ";
        cout << "] del area de swapping" << endl;
    }
}

void wipeMemory(){
    M.clear();
    S.clear();
}

int main() {
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
            processL(cmds[i].getProcessNumber());
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

