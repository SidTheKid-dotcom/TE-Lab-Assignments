#include <iostream>
using namespace std;

class MemoryAllocation {
    int nBlocks, nProcesses;
    int blocks[20], process[20], allocation[20];

public:
    void input() {
        cout << "Enter number of memory blocks: ";
        cin >> nBlocks;
        cout << "Enter size of each block:\n";
        for (int i = 0; i < nBlocks; i++)
            cin >> blocks[i];

        cout << "Enter number of processes: ";
        cin >> nProcesses;
        cout << "Enter size of each process:\n";
        for (int i = 0; i < nProcesses; i++)
            cin >> process[i];

        for (int i = 0; i < nProcesses; i++)
            allocation[i] = -1; // initially no process is allocated
    }

    void showBlocks(int b[]) {
        cout << "Blocks now: ";
        for (int i = 0; i < nBlocks; i++) {
            cout << "[" << i+1 << ":" << b[i] << "] ";
        }
        cout << endl;
    }

    void display() {
        cout << "\nProcess No.\tProcess Size\tBlock No.\n";
        for (int i = 0; i < nProcesses; i++) {
            cout << " " << i + 1 << "\t\t" << process[i] << "\t\t";
            if (allocation[i] != -1)
                cout << allocation[i] + 1;
            else
                cout << "Not Allocated";
            cout << endl;
        }
    }

    void firstFit() {
        int tempBlocks[20];
        for (int i = 0; i < nBlocks; i++) tempBlocks[i] = blocks[i];
        for (int i = 0; i < nProcesses; i++) allocation[i] = -1;

        cout << "\n--- First Fit Allocation ---\n";
        showBlocks(tempBlocks);
        for (int i = 0; i < nProcesses; i++) {
            for (int j = 0; j < nBlocks; j++) {
                if (tempBlocks[j] >= process[i]) {
                    allocation[i] = j;
                    tempBlocks[j] -= process[i];
                    cout << "Allocated Process " << i+1 << " (size " << process[i] << ") to Block " << j+1 << endl;
                    showBlocks(tempBlocks);
                    break;
                }
            }
            if (allocation[i] == -1)
                cout << "Process " << i+1 << " (size " << process[i] << ") not allocated\n";
        }
        display();
    }

    void bestFit() {
        int tempBlocks[20];
        for (int i = 0; i < nBlocks; i++) tempBlocks[i] = blocks[i];
        for (int i = 0; i < nProcesses; i++) allocation[i] = -1;

        cout << "\n--- Best Fit Allocation ---\n";
        showBlocks(tempBlocks);
        for (int i = 0; i < nProcesses; i++) {
            int bestIdx = -1;
            for (int j = 0; j < nBlocks; j++) {
                if (tempBlocks[j] >= process[i]) {
                    if (bestIdx == -1 || tempBlocks[j] < tempBlocks[bestIdx])
                        bestIdx = j;
                }
            }
            if (bestIdx != -1) {
                allocation[i] = bestIdx;
                tempBlocks[bestIdx] -= process[i];
                cout << "Allocated Process " << i+1 << " (size " << process[i] << ") to Block " << bestIdx+1 << endl;
                showBlocks(tempBlocks);
            } else {
                cout << "Process " << i+1 << " (size " << process[i] << ") not allocated\n";
            }
        }
        display();
    }

    void worstFit() {
        int tempBlocks[20];
        for (int i = 0; i < nBlocks; i++) tempBlocks[i] = blocks[i];
        for (int i = 0; i < nProcesses; i++) allocation[i] = -1;

        cout << "\n--- Worst Fit Allocation ---\n";
        showBlocks(tempBlocks);
        for (int i = 0; i < nProcesses; i++) {
            int worstIdx = -1;
            for (int j = 0; j < nBlocks; j++) {
                if (tempBlocks[j] >= process[i]) {
                    if (worstIdx == -1 || tempBlocks[j] > tempBlocks[worstIdx])
                        worstIdx = j;
                }
            }
            if (worstIdx != -1) {
                allocation[i] = worstIdx;
                tempBlocks[worstIdx] -= process[i];
                cout << "Allocated Process " << i+1 << " (size " << process[i] << ") to Block " << worstIdx+1 << endl;
                showBlocks(tempBlocks);
            } else {
                cout << "Process " << i+1 << " (size " << process[i] << ") not allocated\n";
            }
        }
        display();
    }

    void nextFit() {
        int tempBlocks[20];
        for (int i = 0; i < nBlocks; i++) tempBlocks[i] = blocks[i];
        for (int i = 0; i < nProcesses; i++) allocation[i] = -1;

        cout << "\n--- Next Fit Allocation ---\n";
        showBlocks(tempBlocks);
        int j = 0;
        for (int i = 0; i < nProcesses; i++) {
            int count = 0;
            bool allocated = false;
            while (count < nBlocks) {
                if (tempBlocks[j] >= process[i]) {
                    allocation[i] = j;
                    tempBlocks[j] -= process[i];
                    cout << "Allocated Process " << i+1 << " (size " << process[i] << ") to Block " << j+1 << endl;
                    showBlocks(tempBlocks);
                    allocated = true;
                    break;
                }
                j = (j + 1) % nBlocks;
                count++;
            }
            if (!allocated)
                cout << "Process " << i+1 << " (size " << process[i] << ") not allocated\n";
        }
        display();
    }
};

int main() {
    MemoryAllocation m;
    m.input();

    m.firstFit();
    m.bestFit();
    m.worstFit();
    m.nextFit();

    return 0;
}