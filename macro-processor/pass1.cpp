#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class MNTEntry {
    public:
    string name;
    int mdtPtr, pntPtr, kpdtPtr;
};

class KPDTEnry {
    public:
    string param;
    string defn;
};

vector<string> split_comma(const string &s) {
    vector<string> result;
    stringstream ss(s);
    string item;

    while (getline(ss, item, ',')) {
        result.push_back(item);
    }

    return result;
}

int main() {
    ifstream fin("source.asm");
    ofstream fmdt("MDT.txt"), fmnt("MNT.txt"), fkp("KPDT.txt"), fpnt("PNT.txt"), finter("intermediate.txt");

    vector<MNTEntry> MNT;
    vector<KPDTEnry> KPDT;
    vector<vector<string>> PNT;
    vector<string> MDT;

    string line;
    while(getline(fin, line)) {
        string t = line;

        if(t == "MACRO") {
            getline(fin, line);
            stringstream ss(line);

            MNTEntry m;
            ss >> m.name;
            m.mdtPtr = (int)MDT.size() + 1; // 1-based
            m.pntPtr = (int)PNT.size() + 1; // 1-based
            m.kpdtPtr = (int)KPDT.size() + 1; // 1-based (may be > kpdt size if none)

            MNT.push_back(m);

            PNT.emplace_back();
            auto &params = PNT.back();
            string rest; getline(ss, rest);

            if(!rest.empty()) {
                vector<string> parts = split_comma(rest);
                for(string part : parts) {
                    if(part[0] == '&') {
                        part = part.substr(1);
                    }
                    if(part.find('=') == string::npos) {
                        params.push_back(part);
                    } else {
                        size_t eqPos = part.find('=');
                        string paramName = part.substr(0, eqPos);
                        string defaultValue = part.substr(eqPos + 1);
                        KPDT.push_back({paramName, defaultValue});
                        params.push_back(paramName);
                    }
                }
            }

            while(getline(fin, line)) {
                if(line == "MEND") {
                    break;
                }
                MDT.push_back(line);
            }
        }
        else {
            finter << line << "\n";
        }
    }

     // Write MNT
    fmnt << "INDEX\tNAME\tMDT_PTR\tPNT_PTR\tKPDT_PTR\n";
    for(size_t i=0;i<MNT.size();++i){
        // update KPDT_PTR to reflect start index for that macro's keyword params.
        // We assume KPDTAB entries are sequentially created; the original MNT format expects a start index.
        // Since we stored kpdtPtr as start when macro was encountered, that value is fine.
        fmnt << (i+1) << "\t" << MNT[i].name << "\t" << MNT[i].mdtPtr << "\t" << MNT[i].pntPtr << "\t" << MNT[i].kpdtPtr << "\n";
    }

    // Write MDT
    fmdt << "INDEX\tINSTRUCTION\n";
    for(size_t i=0;i<MDT.size();++i) fmdt << (i+1) << "\t" << MDT[i] << "\n";

    // Write PNTAB
    fpnt << "MACRO\tPARAMETERS\n";
    for(size_t i=0;i<PNT.size();++i){
        fpnt << (i+1) << "\t";
        for(size_t j=0;j<PNT[i].size();++j){
            if(j) fpnt << " ";
            fpnt << PNT[i][j];
        }
        fpnt << "\n";
    }

    // Write KPDTAB
    fkp << "INDEX\tPARAM\tDEFAULT\n";
    for(size_t i=0;i<KPDT.size();++i) fkp << (i+1) << "\t" << KPDT[i].param << "\t" << KPDT[i].defn << "\n";

    fin.close(); fmnt.close(); fmdt.close(); fpnt.close(); fkp.close(); finter.close();

    return 0;
}