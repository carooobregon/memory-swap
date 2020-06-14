#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath> 
#include <unordered_map> 

#include "Command.h"

using namespace std;

int M[128]; //contigua
int S[256]; //virtual
int contProcesos = 0;
float turnaround = 0.0; 
unordered_map<int, vector<pair < char, int> > > pcb;
vector<float> timestamps;

Command processCmd(string line){
    char type = line[0];
    istringstream stream(line);
    Command cmd(type);
    string currWord;
    //TODO: stoi error handling
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

        case 'E': {
            break;
        }

        case 'F': {
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

void processP(Command cmd){
    //cout << "NBytes = " << cmd.getNBytes() << endl;
    //cout << "sos " << cmd.getNBytes()/16.0 << endl;
    int pages = ceil(cmd.getNBytes()/16.0);
    //cout << "pages = " << pages << endl;
    int count = 0;
    int pNum = cmd.getProcessNumber();
    float timestamp = 0.0;
    contProcesos++;
    vector<pair < char, int> > save;
    vector<int> assignedM;
    vector<int> assignedS;
    vector<pair <int, int> > swapped;
    cout << "Asignar " << cmd.getNBytes() << " bytes al proceso " << pNum << endl;
    //intenta poner el proceso en los espacios vacios de M y S
    for (int i = 0; i < 128 && count != pages; i++) {
        if (M[i] == -1) {
            M[i] = pNum;
            count++;
            timestamp++;
            turnaround++;
            assignedM.push_back(i);
            save.push_back(make_pair('m',i));
        }
    }
    for (int i = 0; i < 256 && count != pages; i++) {
        if (S[i] == -1) {
            S[i] = pNum;
            count++;
            timestamp++;
            turnaround++;
            assignedS.push_back(i);
            save.push_back(make_pair('s',i));
        }
    }
    //Si falta espacio, pon el proceso en los M y S overwritteando otros procesos
    if (pages != count) { //si no cupo en los vacio
        for (int i = 0; i < 256 && count != pages; i++) {
            if (S[i] == -1) {
                swapped.push_back(make_pair(i,S[i]));
                S[i] = pNum;
                count++;
                timestamp++;
                turnaround++;
                assignedS.push_back(i);
                save.push_back(make_pair('s',i));
            }
        }
        for (int i = 0; i < 128 && count != pages; i++) {
            if (M[i] != pNum) {
                swapped.push_back(make_pair(i,M[i]));
                M[i] = pNum;
                count++;
                timestamp++;
                turnaround++;
                assignedM.push_back(i);
                save.push_back(make_pair('m',i));
            }
        }
    }

    pcb[pNum] = save;

    timestamps.push_back(timestamp);
    
    for (int i = 0; i < swapped.size(); i++) {
        cout << "pagina " << swapped[i].first << " del proceso " << swapped[i].second << " swappeada para hacer espacio para el proceso " << pNum;
    }

    if (!assignedM.empty()) {
        cout << "En la memoria M, se asignaron los marcos de pagina ";
        for (int i = 0; i < assignedM.size(); i++) {
            cout << assignedM[i] << ", ";
        }
        cout << "al proceso " << pNum << endl;
    }

    if (!assignedS.empty()) {
        cout << "En la memoria S, se asignaron los marcos de pagina ";
        for (int i = 0; i < assignedS.size(); i++) {
            cout << assignedS[i] << ", ";
        }
        cout << "al proceso " << pNum << endl;
    }
}

void processA(Command cmd){
    int vDir = cmd.getVDir();
    int nProc = cmd.getProcessNumber();
    cout << "Obtener la dirección real correspondiente a la dirección virtual " << vDir << " del proceso " << cmd.getProcessNumber() << endl;
    int dir = floor(vDir/16.0);
    int mod = vDir%16;
    char mem = pcb[nProc][dir].first;
    int marco = pcb[nProc][dir].second;
    float timestamp = 0.0;
    contProcesos++;

    if (cmd.isModified()) {
        timestamp+=0.1;
        turnaround+=0.1;
        cout << "Página " << dir << " del proceso " << nProc << " modificada. " << endl;
    }

    for (int i = 0; i < 128; i++) {
        if (i == dir) {
            timestamp+=0.1;
            turnaround+=0.1;
            cout << "Dirección virtual: " << vDir << " Dirección real: " << vDir << endl;
            timestamps.push_back(timestamp);
            return;
        }
    }

    for (int i = 0; i < 128; i++) {
        if (M[i] == -1) {
            M[i] = nProc;
            pcb[nProc].push_back(make_pair('m',i));
            timestamp+=0.1;
            turnaround+=0.1;
            cout << "Se localizó la página "<< dir << " del proceso " << nProc << " que estaba en la posición " << marco << " de swapping y se cargo al marco " << i << endl;
            cout << "Dirección virtual: " << vDir << " Dirección real: " << (16*i)+mod << endl;
            timestamps.push_back(timestamp);
            return;
        }        
    }

    for (int i = 0; i < 128; i++) {
        if (M[i] != nProc) {
            M[i] = nProc;
            pcb[nProc].push_back(make_pair('m',i));
            timestamp+=0.1;
            turnaround+=0.1;
            cout << "Se localizó la página "<< dir << " del proceso " << nProc << " que estaba en la posición " << marco << " de swapping y se cargo al marco " << i << endl;
            cout << "Dirección virtual: " << vDir << " Dirección real: " << (16*i)+mod << endl;
            timestamps.push_back(timestamp);
            return;
        }        
    }
}

void processL(int nProc){
    cout << "Liberar los marcos de pagina ocupados por el proceso " << nProc << endl;
    vector<int> freed;
    contProcesos++;
    float timestamp = 0;
    for (int i = 0; i < 128; i++) {
        if (M[i] == nProc) {
            M[i] = -1;
            freed.push_back(i);
            turnaround+=0.1;
            timestamp+=0.1;
        }
    }
    if (!freed.empty()) {
        cout << "Se liberan los marcos de memoria real: ";
        for (int i = 0; i < freed.size(); i++) 
            cout << freed[i] << ", ";
        cout << endl;
    }
    freed.clear();
    for (int i = 0; i < 256; i++) {
        if (S[i] == nProc) {
            S[i] = -1;
            freed.push_back(i);
            turnaround+=0.1;
            timestamp+=0.1;
        }
    }
     if (!freed.empty()) {
        cout << "Se liberan los marcos [ ";
        for (int i = 0; i < freed.size(); i++) 
            cout << freed[i] << ", ";
        cout << "] del area de swapping" << endl;
    }
    timestamps.push_back(timestamp);
}

void wipeMemory(){
    for(int i=0; i < timestamps.size(); i++){
        cout << "Turnaround del proceso #" << i+1 << " = " << timestamps[i] << endl;
    }
    timestamps.clear();
    if(contProcesos) cout << "Turnaround promedio = " << turnaround/contProcesos << endl;
    turnaround = 0;
    contProcesos = 0;
    memset(M, -1, sizeof(M));
    memset(S, -1, sizeof(S));
}

int main() {
    wipeMemory();
    vector<Command> cmds;

    cmds = readFile();

    for (int i = 0; i < cmds.size(); i++) {
        cmds[i].print();
        switch (cmds[i].getType())
        {
        case 'P':
            processP(cmds[i]);
            break;
        
        case 'A':
            processA(cmds[i]);
            break;

        case 'L':
            processL(cmds[i].getProcessNumber());
            break;

        case 'C':
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
        cout << endl;
    }
    return 0;

}

