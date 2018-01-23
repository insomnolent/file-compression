#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include "hashtable.cpp"
using namespace std;

// if the 8 byte sequences aren't the same
// check if there's smaller sequences in each sequence that are the same
int checkSmaller(string seq1, string seq2) {
    int overlap = 0;
    int len = (seq1.length()<seq2.length())?seq1.length():seq2.length();
    for (int i = 0; i < len; i++) {
        if (seq1[i] == seq2[i]) {
            overlap++;
        }
        else {
            return overlap;
        }
    }
    return overlap;
}

// creates a delta file from oldf and newf and sets deltaf to it
void createDelta(istream& oldf, istream& newf, ostream& deltaf) {
    char c;
    string oldfile; // reads in file to oldfile
    // while oldf still has characters to read in
    while (oldf.get(c)) {
        oldfile += c;
    }
    string newfile; // reads in file to oldfile
    // while newf still has characters to read in
    while (newf.get(c)) {
        newfile += c;
    }

    //read in sequences of 8 chars from oldfile and put into hash table oldHash
    HashTable oldHash = HashTable();

    int i, OFlen = oldfile.length();
    for (i = 0; i < OFlen; i++) {
        string sequence = oldfile.substr(i, 8);
        // puts both that sequence and offset in the hash table
        oldHash.put(sequence, i);
    }

    int NFsize = newfile.length();
    // looks through each char in newfile to see what's different from oldfile
    for (i = 0; i < NFsize; i++) {
        // sets sequence1 to 8-byte substring of newfile
        string sequence1 = newfile.substr(i, 8);

        // keeps track of the best Offset and best/longest length values
        int bestOffset = 0;
        int bestLen = 0;

        vector<int> v; // the vector of offsets of that sequence
        // to set v equal to a vector of offsets for that sequence
        // if that sequence has a vector of offsets (so possibly more than one)
        // have to determine which one returns the longest consecutive sequence
        int tempOffset = 0;
        int tempLen = 0;

        // find the longest length (bestLen) of consecutive overlapped sequences
        // go through each offset in vector of offsets and see which one has longest
        // consecutive same values between newfile and oldfile.
        // getOffsets returns true if that sequence was found in oldfile's hash table
        if (oldHash.getOffsets(sequence1, v)) {
            int vsize = v.size();

            // limit will account for files like strange.txt where there are
            // sequences with many offsets. If the vector of offsets v is too
            // long then it'll take too long to run. So if vsize is greater than
            // the file length / 8 then it sets limit to true and later returns
            // the first number of chars to be copied that's greater than 25 bytes
            bool limit = false;
            if (vsize > newfile.length()/8) {
                limit = true;
            }
            // look at each offset in the vector of offsets returned
            // determine which offset results in longest consecutive string
            for (int j = 0; j < vsize; j++) {
                // set temporary offset to that offset value from vector v
                tempOffset = v[j];
                // reset tempLen to 8 to account for 8 sequences already matching
                tempLen = 8;
                // check seq1 from newfile with seq2 from oldfile
                string sequence2 = oldfile.substr(tempOffset, 8);

                string seq1 = sequence1;
                string seq2 = sequence2;
                // while the sequences are matching and while indices are less than each file's sizes
                while (seq1 == seq2 && (i+tempLen) < NFsize && (tempOffset+tempLen) < OFlen) {
                    seq1 = newfile[i + tempLen];
                    seq2 = oldfile[tempOffset + tempLen];
                    // if sequences don't match then add the number of consecutive chars between seq1 and seq2 that overlap
                    if (seq1 != seq2) {
                        tempLen += checkSmaller(seq1, seq2);
                        break;
                    }
                    tempLen++;
                }
                // if temporary length of sequence from that offset is greater than bestLen
                // set bestLen to that tempLen
                if (tempLen > bestLen) {
                    bestOffset = tempOffset;
                    bestLen = tempLen;
                    // if vsize is really big then don't want to keep checking through every single offset
                    // limits runtime by returning an adequately sized sequence to copy
                    if (limit) {
                        if (bestLen > 20) {
                            break;
                        }
                    }
                }
            }
            if (bestLen != 0) {
                deltaf << "C" << bestLen << "," << bestOffset;
            }
        }
        // if the sequence doesn't match - so then will have to copy stuff from newfile
        else {
            string toAdd = "";
            string seq1 = sequence1;
            // while seq1 isn't found in hash table
            while ((i + bestLen) < NFsize && !oldHash.checkSeq(seq1)) {
                toAdd += newfile[i + bestLen];
                bestLen++;
                // look at the next 8-byte sequence in newfile
                seq1 = newfile.substr(i + bestLen, 8);
            }
            // bestLen should be the length of string from newfile to add
            if (bestLen != 0) {
                deltaf << "A" << bestLen << ":" << toAdd;
            }
        }
        // increments i by length of string that was just copied or added
        i += bestLen - 1;
    }
}

bool getInt(istream& inf, int& n) {
    char ch;
    if (!inf.get(ch)  ||  !isascii(ch) || !isdigit(ch))
        return false;
    inf.unget();
    inf >> n;
    return true;
}

bool getCommand(istream& inf, char& cmd, int& length, int& offset) {
    if (!inf.get(cmd)  ||  (cmd == '\n'  &&  !inf.get(cmd)) )
    {
        cmd = 'x';  // signals end of file
        return true;
    }
    char ch;
    switch (cmd)
    {
        case 'A':
            return getInt(inf, length) && inf.get(ch) && ch == ':';
        case 'C':
            return getInt(inf, length) && inf.get(ch) && ch == ',' && getInt(inf, offset);
    }
    return false;
}

    // The function returns true if the operation succeeds. If it fails because the delta file is malformed
    // (e.g., there's a character other than an A or C where a command is expected, or an offset or length
    // is invalid), the function returns false. If length of command is 0, the function returns false.
bool applyDelta(istream &oldf, istream &deltaf, ostream &newf) {
    char command = 0;
    int length = 0;
    int offset = 0;
    int n = 0;
    char c;
    string oldfile; // reads in file to oldfile
    // while ofile still has characters to read in
    while (oldf.get(c)) {
        oldfile += c;
    }
    // while the deltafile isn't done
    while (command != 'x') {
        if (getCommand(deltaf, command, length, offset)) {
            if (length == 0) {
                return false;
            }
            // if there's an add command
            if (command == 'A') {
                string toAdd = "";
                char c;
                for (int i = 0; i < length; i++) {
                    deltaf.get(c);
                    toAdd += c;
                }
                newf << toAdd;
            }
            // if there's a copy command
            else if (command == 'C') {
                string toCopy = oldfile.substr(offset, length);
                newf << toCopy;
            }
        }
        // if deltafile is malformed
        else {
            return false;
        }
    }
    return true;
}