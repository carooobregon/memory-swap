#ifndef Command_h
#define Command_h

class Command {
public:
    Command(char type);
    void setP(int nBytes, int nProc);
    void setA(int vDir, int nProc, bool modify);
    void setL(int nProc);

    
private:
    char type;
    int nBytes; //for P process
    int nProc;
    bool modify; //for A process
    int vDir; //for A process
};

Command::Command(char type) {
    this -> type = type;
    nProc = -1;
    modify = false;
    vDir = -1;
    nBytes = -1;
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

#endif /* Command_h */