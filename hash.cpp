#include <functional>
#include <iostream>
#include <vector>
using namespace std;

const int TABLE_SIZE = 1000;

class HashTable {
public:
    HashTable(int length) {
        for (int i = 0; i < TABLE_SIZE; i++)
            // creates hash table with a vector of strings in each index
            table[i] = vector<string>();
        // initialize offsets to that length
        //offsets[length];
    }

    // get sequence that that offset/index points to in the hashtable
    string getSequence(int offset) {
        //cout << "offsets " << *offsets[0] << endl;
        return *offsets[offset];
    }

    int getOffset(string seq) {

    }

    // puts a sequence in the hash table
    void put(string seq) {
        // makes index (hash) of that sequence
        hash<string> hashstr;
        // put seq through hash function and find remainder (with modulus operator) if the returning number is really big
        string* str = new string(seq);

        int hash = (hashstr(seq) % TABLE_SIZE);
        table[hash].push_back(*str);
        int lastInd = table[hash].size()-1;
        offsets.push_back(str);
        //cout << *offsets[lastInd] << endl;
    }
    ~HashTable() {
        for (int i = 0; i < offsets.size(); i++) {
            delete offsets[i];
        }
    }
private:
    // hash table of vectors of strings with length TABLE_SIZE
    vector<string> table[TABLE_SIZE];
    // vector of offsets that each contain a pointer to a sequence
    vector<string*> offsets;
    //string* offsets[TABLE_SIZE];
};


bool runtest(string oldname, string newname, string deltaname, string newname2)
{
    ifstream oldfile(oldname);
    if (!oldfile)
    {
        cerr << "Cannot open " << oldname << endl;
        return false;
    }
    ifstream newfile(newname);
    if (!newfile)
    {
        cerr << "Cannot open " << newname << endl;
        return false;
    }
    ofstream deltafile(deltaname);
    if (!deltafile)
    {
        cerr << "Cannot create " << deltaname << endl;
        return false;
    }
    createDelta(oldfile, newfile, deltafile);
    deltafile.close();

    oldfile.clear();   // clear the end of file condition
    oldfile.seekg(0);  // reset back to beginning of the file
    ifstream deltafile2(deltaname);
    if (!deltafile2)
    {
        cerr << "Cannot read the " << deltaname << " that was just created!" << endl;
        return false;
    }
    ofstream newfile2(newname2);
    if (!newfile2)
    {
        cerr << "Cannot create " << newname2 << endl;
        return false;
    }
    assert(applyDelta(oldfile, deltafile2, newfile2));
    cerr << "You must compare " << newname << " and " << newname2 << endl;
    cerr << "If they are not identical, you failed this test." << endl;
    return true;
}


int main() {
//     for testing smallmart
//    string ofile = "/Users/christinesun/ClionProjects/CS32_project4/p4test/file1/test1.txt";
//    string nfile = "/Users/christinesun/ClionProjects/CS32_project4/p4test/file2/test2.txt";
//    string dfile = "/Users/christinesun/ClionProjects/CS32_project4/p4test/delta/results_test.txt";
//    string nfile2 = "/Users/christinesun/ClionProjects/CS32_project4/p4test/newfile/newfile_test.txt";
//
//     for testing smallmart
//    string ofile = "/Users/christinesun/ClionProjects/CS32_project4/p4test/file1/smallmart1.txt";
//    string nfile = "/Users/christinesun/ClionProjects/CS32_project4/p4test/file2/smallmart2.txt";
//    string dfile = "/Users/christinesun/ClionProjects/CS32_project4/p4test/delta/results_smallmart.txt";
//    string nfile2 = "/Users/christinesun/ClionProjects/CS32_project4/p4test/newfile/newfile_smallmart.txt";
//
//    // for testing warandpeace
//    string ofile = "/Users/christinesun/ClionProjects/CS32_project4/p4test/file1/warandpeace1.txt";
//    string nfile = "/Users/christinesun/ClionProjects/CS32_project4/p4test/file2/warandpeace2.txt";
//    string dfile = "/Users/christinesun/ClionProjects/CS32_project4/p4test/delta/results_warandpeace.txt";
//    string nfile2 = "/Users/christinesun/ClionProjects/CS32_project4/p4test/newfile/newfile_warandpeace.txt";
//
//      for testing strange
    string ofile = "/Users/christinesun/ClionProjects/CS32_project4/p4test/file1/strange1.txt";
    string nfile = "/Users/christinesun/ClionProjects/CS32_project4/p4test/file2/strange2.txt";
    string dfile = "/Users/christinesun/ClionProjects/CS32_project4/p4test/delta/results_strange.txt";
    string nfile2 = "/Users/christinesun/ClionProjects/CS32_project4/p4test/newfile/newfile_strange.txt";
//
//     for testing greeneggs
//    string ofile = "/Users/christinesun/ClionProjects/CS32_project4/p4test/file1/greeneggs1.txt";
//    string nfile = "/Users/christinesun/ClionProjects/CS32_project4/p4test/file2/greeneggs2.txt";
//    string dfile = "/Users/christinesun/ClionProjects/CS32_project4/p4test/delta/results_greeneggs.txt";
//    string nfile2 = "/Users/christinesun/ClionProjects/CS32_project4/p4test/newfile/newfile_greeneggs.txt";
    assert(runtest(ofile, nfile, dfile, nfile2));

}
