#include <functional>
#include <iostream>
#include <vector>
using namespace std;

const int TABLE_SIZE = 10000;

struct Bucket {
public:
    Bucket() { }
    Bucket(string s, int o) {
        seq = s;
        offset.push_back(o); // pushes an offset value into vector of offset ints
    }
    // returns sequence
    string getSequence() {
        return seq;
    }
    // returns vector of all offsets of that sequence
    vector<int> getAllOffsets() {
        return offset;
    }
    // adds an offset value (offs) to vector offset
    void addOffset(int offs) {
        offset.push_back(offs);
    }
private:
    // keeps track of each sequence
    string seq;
    // a vector of all possible offsets for that sequence
    vector<int> offset;
};

class HashTable {
public:
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++)
            // creates hash table with a vector of Buckets in each index
            table[i] = vector<Bucket>();
    }

    // returns true and sets offsets to a vector of all offsets for that sequence
    // returns false if sequence not found or has no offsets
    bool getOffsets(string seq, vector<int>& offsets) {
        // finds index (hash) of that sequence
        hash<string> hashstr;
        int hash = (hashstr(seq) % TABLE_SIZE);
        // if nothing at that index of table then return false (for 0 offsets)
        if (table[hash].size() == 0) {
            return false;
        }
        else { // if there were collisions when putting sequences in hash table
            // if that index of hash table has 1 or more Buckets
            int i = 0;
            // loop through that index until that sequence is reached
            while (i < table[hash].size() && table[hash][i].getSequence() != seq) {
                i++;
            }
            if (i != table[hash].size()) {
                // set offsets to vector of all offsets at that index
                offsets = table[hash][i].getAllOffsets();
                return true;
            }
            // if sequence isn't already in that index then return false
            return false;
        }
    }

    // checks if a certain sequence is in the hashtable
    bool checkSeq(string seq) {
        hash<string> hashstr;
        int hash = (hashstr(seq) % TABLE_SIZE);
        if (table[hash].size() == 0) {
            return false;
        }
        else { // if there were collisions when putting sequences in hash table
            int i = 0;
            // loop through that index until that sequence is reached
            while ( i < table[hash].size() && table[hash][i].getSequence() != seq) {
                i++;
            }
            // if i didn't reach end of vector of offsets
            if (i != table[hash].size()) {
                return true;
            }
            // if sequence isn't already in that index then return false
            return false;
        }
    }

    // puts a Bucket in the hash table
    void put(string seq, int offset) {
        // finds index (hash) of that sequence
        hash<string> hashstr;
        int hash = (hashstr(seq) % TABLE_SIZE);
        //cerr << "sequence " << seq << " with hash " << hash << endl;
        // if nothing at that index of table
        if (table[hash].size() == 0) {
            // create new Bucket to put into table at that index
            table[hash].push_back(Bucket(seq, offset));
            return;
        }
        else if (table[hash][0].getSequence() == seq) {
            // if Bucket's sequence at that index matches seq, then add another offset to it
            table[hash][0].addOffset(offset);
            return;
        }
        else {
            int i = 0;
            // loop through that index until that sequence is reached
            while ( i < table[hash].size() && table[hash][i].getSequence() != seq) {
                i++;
            }
            if (i != table[hash].size()) {
                // add another offset value the sequence in that index
                // indicates that there's multiple offsets for that sequence in the file
                table[hash][i].addOffset(offset);
                return;
            }
            // if sequence isn't already in that index then add new Bucket(key, offset)
            table[hash].push_back(Bucket(seq, offset));
            return;
        }
    }
private:
    // creates a hashtable of Buckets
    vector<Bucket> table[TABLE_SIZE];
};

