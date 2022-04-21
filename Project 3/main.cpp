#include "contestedpercent.cpp"
#include <chrono>
using namespace std;

int main() {
    bool dataStructureChosen = false;
    string option;
    hashedPQ HPQ;
    SplayTree urbanTree;
    UNode* rootNode = nullptr;
    while (!dataStructureChosen) { //choose the data structure
        cout << "Choose the data structure to store the dataset:" << endl;
        cout << "0 - Splay Tree" << endl << "1 - Priority Queue and Hashmap" << endl;
        cin >> option;
        if(option == "0") { //populate splay tree
        
            dataStructureChosen = true;
            auto start = chrono::high_resolution_clock::now();
            rootNode = urbanTree.populateSplay("updated-urbandict2.csv", rootNode, urbanTree);
            auto stop = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Time taken to populate Splay Tree: " << duration.count() << " milliseconds" << endl;
            
        }
        else if(option == "1") { //populate the PQ and Hashmap
            dataStructureChosen = true;
            auto start = chrono::high_resolution_clock::now();
            HPQ.populate("updated-urbandict2.csv");
            auto stop = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
            cout << "Time taken to populate hashmap + PQ: " << duration.count() << " milliseconds" << endl;
        }
        else {
            cout << "invalid input" << endl;
        }
    }
    bool exited = false;
    string option2;
    if(option == "0") { //Splay tree functionality
        while(!exited) {
            cout << "Select an action:" << endl;
            cout << "0 - Search for a specific word" << endl;
            cout << "1 - View most popular words" << endl;
            cout << "2 - Exit the program" << endl;
            cin >> option2;
            if(option2 == "0") { //search word
                string wordSearch;
                cout << "Type in a word to search: ";
                cin >> wordSearch;
                auto start = chrono::high_resolution_clock::now();
                rootNode = urbanTree.SearchByWord(wordSearch, rootNode);
                cout << setw(10) << "Word ID" << setw(10)
                << "Upvotes" << setw(10)
                << "Downvotes" << setw(60)
                << "Definition" << setw(30)
                << "Time Posted " << endl;
                cout << setw(10) << rootNode->wordId << setw(10)
                << rootNode->upVotes << setw(10)
                << rootNode->downVotes << setw(60)
                << rootNode->definition << setw(30)
                << ctime(&(rootNode->time)) << endl;
                auto stop = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
                cout << "Time taken to access word: " << duration.count() << " milliseconds" << endl;
            }else if (option2 == "1") {
                string topCount;
                bool validInput = true;
                cout << "Enter how many words you would like to view: ";
                cin >> topCount;
                for(int i = 0; i < topCount.length(); i++) {
                    if(!isdigit(topCount[i])) {
                        cout << "invalid input";
                    }
                }
                if(validInput) {
                    urbanTree.TopWords(stoi(topCount), rootNode); // Calls afunction to print top upvoted words
                }
            }else if(option2 == "2") { //exit program
                exited = true;
            }
            else {
                cout << "invalid input" << endl;
            }
        }
    }
    if(option == "1") { //PQ and Hashmap functionality
        while(!exited) {
            cout << "Select an action:" << endl;
            cout << "0 - Search for a specific word" << endl;
            cout << "1 - View most popular words" << endl;
            cout << "2 - Exit the program" << endl;
            cin >> option2;
            if(option2 == "0") { //search word
                string wordSearch;
                cout << "Type in a word to search: ";
                cin >> wordSearch;
                auto start = chrono::high_resolution_clock::now();
                auto defs = HPQ.searchWord(wordSearch);
                auto stop = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
                cout << "Time taken to access word: " << duration.count() << " milliseconds" << endl;
                start = chrono::high_resolution_clock::now();
                contestedPercent(defs);
                stop = chrono::high_resolution_clock::now();
                duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
                cout << "Time taken to find contested value: " << duration.count() << " milliseconds" << endl;
            }
            else if(option2 == "1") { //get top words
                string topCount;
                bool validInput = true;
                cout << "Enter how many words you would like to view: ";
                cin >> topCount;
                for(int i = 0; i < topCount.length(); i++) {
                    if(!isdigit(topCount[i])) {
                        cout << "invalid input";
                    }
                }
                if(validInput) {
                    HPQ.viewTopWords(stoi(topCount));
                }
            }
            else if(option2 == "2") { //exit program
                exited = true;
            }
            else {
                cout << "invalid input" << endl;
            }
        }
    }
    cout << "Exited program. Goodbye!" << endl;
    return 0;
}