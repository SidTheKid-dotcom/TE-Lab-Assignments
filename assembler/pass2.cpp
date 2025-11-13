#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Symbol {
public:
    string name;
    int address;
};

class Literal {
public:
    string name;
    int address;
};

class AssemblerPass2 {
public:
    public:
    Symbol symtab[20];
    Literal littab[20];
    int symcount, litcount = 0;

    AssemblerPass2() {
        initializeSymbolTable();
        initializeLiteralTable();
    }

    void initializeSymbolTable() {
        ifstream ins("symbol.txt");
        string line;
        symcount = 0;
        while (getline(ins, line)) {
            stringstream ss(line);
            string name;
            int address;
            ss >> name >> address;
            if(name.empty()) continue;
            if(name == "Symbol") continue;
            symtab[symcount].name = name;
            symtab[symcount].address = address;
            symcount++;
        }
    }

    void initializeLiteralTable() {
        ifstream ins("literal.txt");
        string line;
        litcount = 0;
        while (getline(ins, line)) {
            stringstream ss(line);
            string name;
            int address;
            ss >> name >> address;
            if(name.empty()) continue;
            if(name == "Literal") continue;
            littab[litcount].name = name[2];
            littab[litcount].address = address;
            litcount++;
        }
    }

    void process(string input, string output) {
        ifstream fin(input);
        ofstream fout(output);
        string line;

        while (getline(fin, line)) {
            if (line.empty()) continue;

            // Skip assembler directives (AD)
            if (line.find("(AD") != string::npos) continue;

            stringstream ss(line);
            string token;
            string LC = "", opcode = "00", reg = "0", mem = "000";

            // Get LC first
            ss >> token;
            LC = token.substr(1, token.size() - 2); // (100) -> 100

            // Process the rest
            while (ss >> token) {
                if (token.find("(IS") != string::npos) {
                    opcode = token.substr(4, 2); // (IS,04) -> 04
                }
                else if (token.find("(RG") != string::npos) {
                    reg = token.substr(4, 1); // (RG,1) -> 1
                }
                else if (token.find("(S") != string::npos) {
                    int index = stoi(token.substr(3, token.size() - 4));
                    if (index >= 0 && index < symcount)
                        mem = to_string(symtab[index].address);
                }
                else if (token.find("(L") != string::npos) {
                    int index = stoi(token.substr(3, token.size() - 4));
                    if (index >= 0 && index < litcount)
                        mem = to_string(littab[index].address);
                }
                else if (token.find("(C") != string::npos) {
                    mem = token.substr(3, token.size() - 4); // (C,3) -> 3
                }
                else if (token.find("(DL,02)") != string::npos) {
                    opcode = "00";
                    reg = "0";
                }
            }

            // Output: LC OPCODE REGISTER MEM
            fout << LC << "\t" << opcode << "\t" << reg << "\t" << mem << "\n";
        }

        fin.close();
        fout.close();
    }
};

int main() {
    AssemblerPass2 assembler;
    assembler.initializeLiteralTable();
    assembler.initializeSymbolTable();

    assembler.process("intermediate.txt", "output.txt");

    cout << "✅ Pass 2 complete! Check output.txt\n";
    return 0;
}
