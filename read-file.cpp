/**
 * @readFile
 * 
 * Function that takes input file and setup command list to execute
 */
vector<Command> readFile(){
  vector<Command> cmds;
  string line;
  ifstream file;
  file.open("ArchivoTrabajo.txt");

  if (file.fail()) {
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