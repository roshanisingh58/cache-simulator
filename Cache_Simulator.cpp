/* NAME:- Roshani Singh        COURSE:- Information Technology       ROLL NUMBER:- 58 */
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

#define HIT 1
#define MISS 0
#define READ 0
#define WRITE 1

using namespace std;

static int LRUCtr = 1;

class Memory {
public:
    int readblock(unsigned int /*Address*/) { return 1; }
    int writeblock(unsigned int /*Address*/) { return 1; }
};

Memory M;

class Cache {
private:
    int sets = 0, asso = 0, lineSize = 0;
    int hit_counter = 0, miss_counter = 0;

    int **TAG = nullptr;
    int **LRU = nullptr;
    bool **Dirty = nullptr;

public:
    int get_hit() const { return hit_counter; }
    int get_miss() const { return miss_counter; }

    Cache() = default;

    void CacheInit(int s, int a, int ls) {
        sets = s;
        asso = a;
        lineSize = ls;
        hit_counter = 0;
        miss_counter = 0;

        TAG = new int*[sets];
        LRU = new int*[sets];
        Dirty = new bool*[sets];

        for (int i = 0; i < sets; i++) {
            TAG[i] = new int[asso];
            LRU[i] = new int[asso];
            Dirty[i] = new bool[asso];

            for (int j = 0; j < asso; j++) {
                TAG[i][j] = -1;        // empty
                LRU[i][j] = 0;         // not used yet
                Dirty[i][j] = false;   // clean
            }
        }
    }

    ~Cache() {
        if (TAG) {
            for (int i = 0; i < sets; i++) delete[] TAG[i];
            delete[] TAG;
        }
        if (LRU) {
            for (int i = 0; i < sets; i++) delete[] LRU[i];
            delete[] LRU;
        }
        if (Dirty) {
            for (int i = 0; i < sets; i++) delete[] Dirty[i];
            delete[] Dirty;
        }
    }

    int Access(unsigned int Address, bool RW) {
        int index = (Address / lineSize) % sets;
        int tag = (Address / lineSize) / sets;

        // HIT check (parallel in real cache)
        for (int i = 0; i < asso; i++) {
            if (TAG[index][i] == tag) {
                hit_counter++;
                LRU[index][i] = LRUCtr++;
                if (RW == WRITE) Dirty[index][i] = true;
                return HIT;
            }
        }

        // MISS
        miss_counter++;

        // Find LRU line (least recently used)
        int lru_index = 0;
        for (int i = 1; i < asso; i++) {
            if (LRU[index][i] < LRU[index][lru_index]) lru_index = i;
        }

        // If victim is dirty, write back old block
        if (Dirty[index][lru_index] && TAG[index][lru_index] != -1) {
            int oldTag = TAG[index][lru_index];
            unsigned int oldAddr = (unsigned int)(oldTag * sets * lineSize + index * lineSize);
            M.writeblock(oldAddr);
        }

        // Bring new block
        M.readblock(Address);

        TAG[index][lru_index] = tag;
        LRU[index][lru_index] = LRUCtr++;
        Dirty[index][lru_index] = (RW == WRITE);

        return MISS;
    }
};

int main() {
    const unsigned int MaxAddress = 1u << 30;

    Cache L;
    L.CacheInit(128, 4, 64);

    ofstream TF("tracefile.txt");
    if (!TF) {
        cout << "Error: tracefile.txt cannot be created.\n";
        return 0;
    }

    srand((unsigned)time(NULL));
    unsigned int Address = rand() % MaxAddress;

    for (int CPUCycle = 0; CPUCycle < 5000; CPUCycle += 4) {
        if ((rand() % 100) < 20)
            Address = rand() % MaxAddress;   // 20% jump
        else
            Address += 4;                    // 80% sequential

        bool RW = ((rand() % 100) < 20) ? WRITE : READ;

        int hit = L.Access(Address, RW);

        TF << CPUCycle << " " << std::hex << Address << std::dec << " " << RW
           << " " << (hit == HIT ? "HIT" : "MISS") << "\n";
    }

    int totalAccesses = L.get_hit() + L.get_miss();
    double hitRate = totalAccesses ? (double)L.get_hit() / totalAccesses : 0.0;
    double missRate = totalAccesses ? (double)L.get_miss() / totalAccesses : 0.0;

    const int HIT_TIME = 2;
    const int MISS_PENALTY = 400;

    double amat = hitRate * HIT_TIME + missRate * MISS_PENALTY;
    double latencyReduction = ((400.0 - amat) / 400.0) * 100.0;

    cout << "\n================ Cache Simulation Results ================\n";
    cout << "Total Accesses     : " << totalAccesses << "\n";
    cout << "Cache Hits         : " << L.get_hit() << "\n";
    cout << "Cache Misses       : " << L.get_miss() << "\n";
    cout << "Hit Rate           : " << hitRate * 100 << " %\n";
    cout << "Miss Rate          : " << missRate * 100 << " %\n";
    cout << "AMAT               : " << amat << " cycles\n";
    cout << "Latency Reduction  : " << latencyReduction << " %\n";
    cout << "==========================================================\n";
    cout << "\nMy work is done\n";
    return 0;
}
