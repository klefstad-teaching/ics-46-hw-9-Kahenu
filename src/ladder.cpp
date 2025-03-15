#include "ladder.h"

void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg << " (" << word1 << ", " << word2 << ")" << endl;
    exit(1);
}

bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    int m = str1.length();
    int n = str2.length();
    
    if (abs(m - n) > d) {
        return false;
    }
    
    if (m < n) {
        return edit_distance_within(str2, str1, d);
    }
    
    vector<int> current(n + 1);
    vector<int> previous(n + 1);
    
    for (int j = 0; j <= n; j++) {
        previous[j] = j;
    }
    
    for (int i = 1; i <= m; i++) {
        current[0] = i;
        for (int j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                current[j] = previous[j - 1];
            } else {
                current[j] = 1 + min(previous[j - 1],  
                                 min(previous[j], current[j - 1]));
            }
        }
        previous = current;
    }
    
    return previous[n] <= d;
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (begin_word == end_word) {
        vector<string> single_word = {begin_word};
        return single_word;
    }
    
    queue<vector<string>> ladder_queue;
    set<string> visited;
    
    vector<string> initial_ladder = {begin_word};
    ladder_queue.push(initial_ladder);
    visited.insert(begin_word);
    
    while (!ladder_queue.empty()) {
        vector<string> current_ladder = ladder_queue.front();
        ladder_queue.pop();
        
        string last_word = current_ladder.back();
        
        for (const string& word : word_list) {
            if (is_adjacent(last_word, word) && visited.find(word) == visited.end()) {
                visited.insert(word);
                
                vector<string> new_ladder = current_ladder;
                new_ladder.push_back(word);
                
                if (word == end_word) {
                    return new_ladder;
                }
                
                ladder_queue.push(new_ladder);
            }
        }
    }
    
    return {};
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream in(file_name);
    if (!in) {
        cerr << "Error: Cannot open file " << file_name << endl;
        exit(1);
    }
    
    string word;
    while (in >> word) {
        for (char& c : word) {
            c = tolower(c);
        }
        word_list.insert(word);
    }
    
    in.close();
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    
    cout << "Word ladder found: ";
    for (size_t i = 0; i < ladder.size(); ++i) {
        cout << ladder[i];
        if (i < ladder.size() - 1) {
            cout << " ";
        }
    }
    cout << " " << endl;
}

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "src/words.txt");
    
    #define my_assert(e) {cout << #e << ((e) ? " passed" : " failed") << endl;}
    
    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}