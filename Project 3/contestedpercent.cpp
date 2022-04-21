#include <vector>
#include <map>
#include <set>
#include <string>
#include <regex>
#include <algorithm>
#include <regex>
#include <iterator>
#include <iostream>
#include <sstream>
#include "hashedPQ.cpp"
#include "SplayTree.cpp"
using namespace std;

set<string> constructExcluded() {
    set<string> excluded ({
      "word","is","was","are","be","have","had","were","will","do","has","would",
      "am","could","been","did","a","an","the","so","also","of","to","in","for","on",
      "with","at","from","by","about","into","though","you","that","it","he","his","they",
      "i","this","what","we","your","which","she","their","them","these","her","him","my",
      "who","its","me","our","us","something","those","and","as","or","but","if","than","because","while","another"  
    });
    return excluded;
}

string clean(string def) {
    //remove punctuation and special characters
    //transform into all lowercase
    const regex extracting("[^A-Za-z0-9\t ]+");
    string cleaned;
    regex_replace(back_inserter(cleaned), def.begin(), def.end(), extracting, "", std::regex_constants::match_any);
    transform(cleaned.begin(), cleaned.end(), cleaned.begin(), ::tolower);

    return cleaned;
}

// given a term, find and count all that match it

void contestedPercent(vector<Node*>& defs) {
    set<string> excluded = constructExcluded();

    if (defs.size() == 0) {
        cout << "Term not found." << endl;
        return;
    }
    else if (defs.size() == 1) {
        cout << "No other definition found, so uncontested." << endl;
    }
    else {
        
        set<string> wordSoup;
        map<string, int> sharing;
        map<int, vector<string>> sharedCount;
        map<string, int> topFive;
               

        for (auto iter = defs.begin(); iter != defs.end(); iter++) {
            
            // take each definition and put the words into a set
            set<string> def;
            def.clear();
            stringstream wdef((*iter)->definition);
            wdef.clear();
            string word = "";
            
            while (wdef >> word) {
                def.insert(clean(word));
            }
            
            //set operations time
            
            //temp = def - excluded
            set<string> temp;
            set_difference(def.begin(), def.end(), excluded.begin(), excluded.end(), inserter(temp, temp.begin()));
            
            //shared = def n wordSoup
            set<string> shared;
            set_intersection(temp.begin(),temp.end(), wordSoup.begin(), wordSoup.end(), inserter(shared, shared.begin()));
            
            //update sharing;
            for (auto it = shared.begin(); it != shared.end(); it++) {
                //+1 if already there
                //+2 if not
                if (sharing.find(*it) != sharing.end()) {
                    sharing.find(*it)++;
                }
                else {
                    sharing[*it] = 2;
                }
            }
                
            //wordSoup = def U wordSoup
            set_union(def.begin(),def.end(), wordSoup.begin(), wordSoup.end(), inserter(wordSoup, wordSoup.begin()));
        
        }

        //convert sharing to sharedCount
        for (auto itr = sharing.begin(); itr != sharing.end(); itr++) {
            sharedCount[itr->second].push_back(itr->first);
        }
        //truncate sharedCount -> topFive
        //picking first five words that appeared in the most definitions
        for (auto it = sharedCount.rbegin(); it !=sharedCount.rend(); it++) {
            if (it->second.empty()) {
                continue;
            }
            if (topFive.size() < 5) {
                for (int i = 0; i < min((int)it->second.size(), 5-(int)topFive.size()); i++) {
                    topFive[it->second[i]] = it->first;
                }
            }
        }
        
    // compare values of (up to) 5 most common words to total term count
    //     word count/term count
    //         0-25% very contested
    //         26-50% contested
    //         51-75% not very contested
    //         76-100% pretty much uncontested
    int c = 0;
    float avg;
    for (auto i = topFive.begin(); i!=topFive.end(); i++) {
            cout << c << ". " << i->first << ": found in " << i->second << "/" << defs.size() << " definitions\n";
            avg += i->second/(float)defs.size();  
    }
    cout << "Weighted Average: " << avg/topFive.size() << endl;
    cout << "Key:" << endl;
    cout << "0-25% very contested\n26-50% contested\n51-75% not very contested\n76-100% pretty much uncontested\n";
    }
}

void contestedPercent(vector<UNode*>& defs) {
    set<string> excluded = constructExcluded();

    if (defs.size() == 0) {
        cout << "Term not found." << endl;
        return;
    }
    else if (defs.size() == 1) {
        cout << "No other definition found, so uncontested." << endl;
    }
    else {
        
        set<string> wordSoup;
        map<string, int> sharing;
        map<int, vector<string>> sharedCount;
        map<string, int> topFive;
               

        for (auto iter = defs.begin(); iter != defs.end(); iter++) {
            
            // take each definition and put the words into a set
            set<string> def;
            def.clear();
            stringstream wdef((*iter)->definition);
            wdef.clear();
            string word = "";
            
            while (wdef >> word) {
                def.insert(clean(word));
            }
            
            //set operations time
            
            //temp = def - excluded
            set<string> temp;
            set_difference(def.begin(), def.end(), excluded.begin(), excluded.end(), inserter(temp, temp.begin()));
            
            //shared = def n wordSoup
            set<string> shared;
            set_intersection(temp.begin(),temp.end(), wordSoup.begin(), wordSoup.end(), inserter(shared, shared.begin()));
            
            //update sharing;
            for (auto it = shared.begin(); it != shared.end(); it++) {
                //+1 if already there
                //+2 if not
                if (sharing.find(*it) != sharing.end()) {
                    sharing.find(*it)++;
                }
                else {
                    sharing[*it] = 2;
                }
            }
                
            //wordSoup = def U wordSoup
            set_union(def.begin(),def.end(), wordSoup.begin(), wordSoup.end(), inserter(wordSoup, wordSoup.begin()));
        
        }

        //convert sharing to sharedCount
        for (auto itr = sharing.begin(); itr != sharing.end(); itr++) {
            sharedCount[itr->second].push_back(itr->first);
        }
        //truncate sharedCount -> topFive
        //picking first five words that appeared in the most definitions
        for (auto it = sharedCount.rbegin(); it !=sharedCount.rend(); it++) {
            if (it->second.empty()) {
                continue;
            }
            if (topFive.size() < 5) {
                for (int i = 0; i < min((int)it->second.size(), 5-(int)topFive.size()); i++) {
                    topFive[it->second[i]] = it->first;
                }
            }
        }
        
    // compare values of (up to) 5 most common words to total term count
    //     word count/term count
    //         0-25% very contested
    //         26-50% contested
    //         51-75% not very contested
    //         76-100% pretty much uncontested
    int c = 0;
    float avg;
    for (auto i = topFive.begin(); i!=topFive.end(); i++) {
            cout << c << ". " << i->first << ": found in " << i->second << "/" << defs.size() << " definitions\n";
            avg += i->second/(float)defs.size();  
    }
    cout << "Weighted Average: " << avg/topFive.size() << endl;
    cout << "Key:" << endl;
    cout << "0-25% very contested\n26-50% contested\n51-75% not very contested\n76-100% pretty much uncontested\n";
    }
}
