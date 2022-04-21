#include <unordered_map>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
using namespace std;

struct Node {
    int word_id;
    string word;
    int up_votes;
    int down_votes;
    string author;
    string definition;
    time_t time;
    Node* next = nullptr;
    Node(int word_id, string word, int up_votes, int down_votes, string author, string definition, time_t time) {
        this->word_id = word_id;
        this->word = word;
        this->up_votes = up_votes;
        this->down_votes = down_votes;
        this->author = author;
        this->definition = definition;
        this->time = time;
    }
};

class hashedPQ {
private:
    unordered_map<string, vector<Node*>> wordMap;
    Node* head = nullptr;
public:
    hashedPQ() {};
    void populate(string filename);
    vector<Node*> searchWord(string word);
    void viewTopWords(int count);
    void insert(int word_id, string word, int up_votes, int down_votes, string author, string definition, time_t time);
    ~hashedPQ();
};

void hashedPQ::populate(string filename) { //populate with entire dataset
    ifstream file;
    file.open(filename);
    string temp;
    int word_id;
    string word;
    int up_votes;
    int down_votes;
    string author;
    string definition;
    if(file.is_open()) {
        getline(file, temp);
        while(getline(file, temp, ',')) {
            word_id = stoi(temp);
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
            insert(word_id, word, up_votes, down_votes, author, definition, time);
        }
    }
    file.close();
}

vector<Node*> hashedPQ::searchWord(string word) {
    vector<Node*> wordEntries = wordMap[word];
    if(!wordEntries.empty()) {
        cout << setw(10) << "Word ID" << setw(10)
        << "Upvotes" << setw(10)
        << "Downvotes" << setw(60)
        << "Definition" << setw(30)
        << "Time Posted " << endl;
        for(int i = 0; i < wordEntries.size(); i++) {
            cout << setw(10) << wordEntries.at(i)->word_id << setw(10)
            << wordEntries.at(i)->up_votes << setw(10)
            << wordEntries.at(i)->down_votes << setw(60)
            << wordEntries.at(i)->definition << setw(30)
            << ctime(&(wordEntries.at(i)->time)) << endl;
        }
    }
    else {
        cout << "Word does not exist!" << endl;
    }
    return wordEntries;
}

void hashedPQ::viewTopWords(int count) {
    cout << setw(10) << "Word ID" << setw(15)
    << "Word" << setw(10)
    << "Upvotes" << setw(10)
    << "Downvotes" << setw(50)
    << "Definition" << setw(30)
    << "Time Posted " << endl;
    Node* currNode = head;
    for(int i = 0; i < count; i++) {
        if(currNode != nullptr) {
            cout << setw(10) << currNode->word_id << setw(15)
            << currNode->word << setw(10)
            << currNode->up_votes << setw(10)
            << currNode->down_votes << setw(50)
            << currNode->definition << setw(30)
            << ctime(&(currNode->time)) << endl;
            currNode = currNode->next;
        }
    }
}

void hashedPQ::insert(int word_id, string word, int up_votes, int down_votes, string author, string definition, time_t time) {
    Node* newNode = new Node(word_id, word, up_votes, down_votes, author, definition, time);
    if(head == nullptr) { //if PQ is empty
        head = newNode;
    }
    else if(up_votes > head->up_votes) { //if new node has more upvotes and therefore greater priority
        newNode->next = head;
        head = newNode;
    }
    else { //else traverse to appropriate position
        Node* currNode = head;
        while(currNode->next != nullptr && currNode->next->up_votes > up_votes)
            currNode = currNode->next;
        newNode->next = currNode->next;
        currNode->next = newNode;
    }
    wordMap[word].push_back(newNode);
}

hashedPQ::~hashedPQ() { //to properly delete PQ
    Node* currNode = head; 
    Node* nextNode;
    while (currNode != nullptr) {
        nextNode = currNode->next;
        delete currNode;
        currNode = nextNode;
    }
}