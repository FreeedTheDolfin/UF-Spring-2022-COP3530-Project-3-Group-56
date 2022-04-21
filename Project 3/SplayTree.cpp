/*
SplayTree.cpp
Takes input from data file and creates a splay tree.
*/
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <iomanip>
#include "SplayTree.h"
using namespace std;

void SplayTree::Test() {
    cout << "Link and compile successful" << endl;
}

// Zigs and zags functions
UNode* SplayTree::RightRightRotate(UNode* inUNode) {
    UNode* tempUNode = inUNode->left;
    inUNode->left = tempUNode->right;
    tempUNode->right = inUNode;
    return tempUNode;
}

UNode* SplayTree::LeftLeftRotate(UNode* inUNode) {
    UNode* tempUNode = inUNode->right;
    inUNode->right = tempUNode->left;
    tempUNode->left = inUNode;
    return tempUNode;
}

// Splay function
UNode* SplayTree::Splay(int wordId, UNode* inUNode) {
    if (inUNode == nullptr) {
        return nullptr;
    }
    UNode tempUNode;
    tempUNode.left = nullptr;
    tempUNode.right = nullptr;
    UNode* subTree1 = &tempUNode;
    UNode* subTree2 = &tempUNode;
    while (true) {
        if (wordId < inUNode->wordId) {
            if (inUNode->left == nullptr) {
                break;
            }
            if (wordId < inUNode->left->wordId) {
                inUNode = RightRightRotate(inUNode);
                if (inUNode->left == nullptr) {
                    break;
                }
            }
            subTree2->left = inUNode;
            subTree2 = subTree2->left;
            inUNode = inUNode->left;
            subTree2->left = nullptr;
        }else if (wordId > inUNode->wordId) {
            if (inUNode->right == nullptr) {
                break;
            }
            if (wordId > inUNode->right->wordId) {
                inUNode = LeftLeftRotate(inUNode);
                if (inUNode->right) {
                    break;
                }
            }
            subTree1->right = inUNode;
            subTree1 = subTree1->right;
            inUNode = inUNode->right;
            subTree1->right = nullptr;
        }else {
            break;
        }
    }
    subTree1->right = inUNode->left;
    subTree2->left = inUNode->right;
    inUNode->left = tempUNode.right;
    inUNode->right = tempUNode.left;
    internalRoot = inUNode;
    return inUNode;
}
// Inserts a node
UNode* SplayTree::Insert(int wordId, string word, int upVotes, int downVotes, string author, string definition, time_t time, UNode* inUNode) {
    UNode* newUNode = new UNode(wordId, word, upVotes, downVotes, author, definition, time);
    sizeOfTree++;
    if (inUNode == nullptr) {
        inUNode = newUNode;
        //newUNode = nullptr;
        return newUNode;
    }
    newUNode = Splay(wordId, newUNode);
    if (wordId < inUNode->wordId) {
        newUNode->left = inUNode->left;
        newUNode->right = inUNode;
        inUNode->left = nullptr;
        inUNode = newUNode;
    }else if (wordId > inUNode->wordId) {
        newUNode->right = inUNode->right;
        newUNode->left = inUNode;
        inUNode->right = nullptr;
        inUNode = newUNode;
    }else {
        return inUNode;
    }
    newUNode = nullptr;
    return inUNode;
}
// Searches by the word ID
UNode* SplayTree::SearchById(int wordId, UNode* inUNode) {
    stack<UNode*> stack;
    UNode* currentUNode = inUNode;
    while (currentUNode != nullptr || stack.empty() == false) {
        while (currentUNode != nullptr) {
            stack.push(currentUNode);
            currentUNode = currentUNode->left;
        }
        currentUNode = stack.top();
        stack.pop();
        if (currentUNode->wordId == wordId) {
            return currentUNode;
        }
        currentUNode = currentUNode->right;
    }
    
  return nullptr;
}
// Searches for the word's id and returns a node
UNode* SplayTree::SearchByWord(string word, UNode* inUUNode) {
    int lookupWord = idLookup.find(word)->second;
    return SearchById(lookupWord, inUUNode);

}

// Gets  the top words, needs to be tested more
vector<UNode*> SplayTree::TopWords(int amount, UNode* root) {
    vector <UNode*> returnVector (amount, root);
    stack<UNode*> stack;
    UNode* currentUNode = root;
    while (currentUNode != nullptr || stack.empty() == false) {
        while (currentUNode != nullptr) {
            stack.push(currentUNode);
            currentUNode = currentUNode->left;
        }
        currentUNode = stack.top();
        stack.pop();
        for (int j = 0; j < amount; j++) {
            if (currentUNode->upVotes > returnVector.at(j)->upVotes) {
                returnVector.at(j) = currentUNode;
                break;
            }
        }
        currentUNode = currentUNode->right;
    }
    cout << setw(10) << "Word ID" << setw(15)
        << "Words" << setw(10)
        << "Upvotes" << setw(10)
        << "Downvotes" << setw(60)
        << "Definition" << setw(30)
        << "Time Posted " << endl;
        for(int i = 0; i < returnVector.size(); i++) {
            cout << setw(10) << returnVector.at(i)->wordId << setw(15)
            << returnVector.at(i)->word << setw(10)
            << returnVector.at(i)->upVotes << setw(10)
            << returnVector.at(i)->downVotes << setw(60)
            << returnVector.at(i)->definition << setw(30)
            << ctime(&(returnVector.at(i)->time)) << endl;
        }

    return returnVector;

}

// Read in the values and then call insert
UNode* SplayTree::populateSplay(string filename, UNode* root, SplayTree &urbanTree) {
    ifstream file;
    file.open(filename);
    string temp;
    string word_id;
    string word;
    int up_votes;
    int down_votes;
    string author;
    string definition;
    if(file.is_open()) {
        getline(file, temp);
        while(getline(file, temp, ',')) {
            word_id = temp;
            getline(file, word, ',');
            getline(file, temp, ',');
            //ensure that temp reads in correct value for up_votes and down_votes
            bool tempIsInt = true;
            if(temp.length() == 0 || temp == "-")
                tempIsInt = false;
            for(int i = 0; i < temp.length(); i++) {
                if(!isdigit(temp[i]) && temp[i] != '-')
                    tempIsInt = false;
            }
            while(!tempIsInt) {
                word = word + temp;
                getline(file, temp, ',');
                tempIsInt = true;
                if(temp.length() == 0 || temp == "-")
                    tempIsInt = false;
                for(int i = 0; i < temp.length(); i++) {
                    if(!isdigit(temp[i]))
                        tempIsInt = false;
                }
            }
            up_votes = stoi(temp);
            getline(file, temp, ',');
            down_votes = stoi(temp);
            getline(file, author, ',');
            getline(file, definition, '\"');
            getline(file, definition, '\"');
            getline(file, temp, ',');
            getline(file, temp);
            struct tm tm;
            istringstream ss(temp);
            ss >> get_time(&tm, "%Y-%m-%dT%H:%M:%S");
            time_t time = mktime(&tm);
            int cutoff = 0;
            for (int i = 0; i < word_id.length(); i++) {
                if (word_id.at(i) == '0') {
                    cutoff++;
                }else {
                    break;
                }
            }
            word_id = word_id.substr(cutoff);
            int actualWordId = stoi(word_id);
            urbanTree.idLookup.emplace(word, actualWordId);
            root = urbanTree.Insert(actualWordId, word, up_votes, down_votes, author, definition, time, root);
        }
    }
    file.close();
    return root;
}