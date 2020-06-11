#ifndef Command_h
#define Command_h

using namespace std;

class Command {
public:
    Command(char type);
    void setP(int nBytes, int nProc);
    void setA(int vDir, int nProc, bool modify);
    void setL(int nProc);
    void setC(string comment);
    void print();
    char getType();
    int getProcessNumber();
    
private:
    char type;
    int nBytes; //for P process
    int nProc;
    bool modify; //for A process
    int vDir; //for A process
    string com; // for C process
};

Command::Command(char type) {
    this -> type = type;
    nProc = -1;
    modify = false;
    vDir = -1;
    nBytes = -1;
    com = "";
}

void Command::setP(int nBytes, int nProc) {
    this -> nBytes = nBytes;
    this -> nProc = nProc;
}

void Command::setA(int vDir, int nProc, bool modify) {
    this -> vDir = vDir;
    this -> nProc = nProc;
    this -> modify = modify;

}

void Command::setL(int nProc) {
    this -> nProc = nProc;
}

void Command::setC(string comment) {
    com = comment;
}

void Command::print() {
    switch (type)
    {
    case 'P':
        cout << type << " " << nBytes << " " << nProc << endl;
        break;

    case 'A':
        cout << type << " " << vDir << " " << nProc << endl;
        break;

    case 'L':
        cout << type << " " << nProc << endl;
        break;

    case 'C':
        cout << type << " " << com << endl;
        break;

    case 'E':
        cout << type << endl;
        break;

    case 'F':
        cout << type << endl;
        break;

    default:
        break;
    }
}

char Command::getType() {
    return type;
}

int Command::getProcessNumber() {
    return nProc;
}

#endif /* Command_h */