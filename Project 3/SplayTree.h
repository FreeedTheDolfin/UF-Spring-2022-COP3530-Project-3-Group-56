#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <map>
using std::string;
using std::map;

struct UNode {
    UNode* left;
    UNode* right;
    int wordId;
    string word;
    int upVotes;
    int downVotes;
    string author;
    string definition;
    time_t time;
    UNode() {
        left = nullptr;
        right = nullptr;
    }
    UNode(int wordId, string word, int upVotes, int downVotes, string author, string definition, time_t time) {
        this->wordId = wordId;
        this->word = word;
        this->upVotes = upVotes;
        this->downVotes = downVotes;
        this->author = author;
        this->definition = definition;
        this->time = time;
        left = nullptr;
        right = nullptr;
    }

};


class SplayTree {
    private:
    UNode* internalRoot = nullptr;
    UNode internalUsedUNode;
    public:
    int sizeOfTree = 0;
    std::map <string, int> idLookup;
    UNode* RightRightRotate(UNode* inUNode);
    UNode* LeftLeftRotate(UNode* k2);
    UNode* Splay(int wordId, UNode* inUNode);
    UNode* Insert(int wordId, string word, int upVotes, int downVotes, string author, string definition, time_t time, UNode* inUNode);
    UNode* SearchByWord(string word, UNode* inUNode);
    UNode* SearchById(int wordId, UNode* inUNode);
    void Test();
    UNode* populateSplay(string filename, UNode* root, SplayTree &urbanTree);
    vector<UNode*> TopWords(int amount, UNode* root);
};

/* Information
 Sources: 
https://www.geeksforgeeks.org/splay-tree-set-1-insert/
https://www.cs.cornell.edu/courses/cs3110/2013sp/recitations/rec08-splay/rec08.html
https://www.cplusplus.com/reference
https://stackoverflow.com/questions/879375/how-can-i-remove-the-leading-zeroes-from-an-integer-generated-by-a-loop-and-stor
*/