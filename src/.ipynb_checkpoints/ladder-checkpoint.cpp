#include "ladder.h"

void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg << " (" << word1 << ", " << word2 << ")" << endl;
    exit(1);
}

bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    if (str1 == str2) return true;
    if (abs((int)str1.length() - (int)str2.length()) > d) return false;
    
    if (str1.length() == str2.length()) {
        int diff = 0;
        for (int i = 0; i < str1.length(); i++) {
            if (str1[i] != str2[i]) diff++;
            if (diff > d) return false;
        }
        return true;
    }
    
    if (abs((int)str1.length() - (int)str2.length()) == 1 && d >= 1) {
        const string& shorter = (str1.length() < str2.length()) ? str1 : str2;
        const string& longer = (str1.length() < str2.length()) ? str2 : str1;
        
        int s = 0, l = 0;
        bool skipped = false;
        
        while (s < shorter.length()) {
            if (l >= longer.length()) return false;
            
            if (shorter[s] != longer[l]) {
                if (skipped) return false;
                skipped = true;
                l++;
            } else {
                s++;
                l++;
            }
        }
        
        return true;
    }
    
    vector<vector<int>> dp(str1.length() + 1, vector<int>(str2.length() + 1));
    
    for (int i = 0; i <= str1.length(); i++) dp[i][0] = i;
    for (int j = 0; j <= str2.length(); j++) dp[0][j] = j;
    
    for (int i = 1; i <= str1.length(); i++) {
        for (int j = 1; j <= str2.length(); j++) {
            if (str1[i-1] == str2[j-1]) dp[i][j] = dp[i-1][j-1];
            else dp[i][j] = 1 + min(min(dp[i-1][j], dp[i][j-1]), dp[i-1][j-1]);
        }
    }
    
    return dp[str1.length()][str2.length()] <= d;
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {

    if (begin_word == end_word) {
        return {begin_word};
    }
    
    if (word_list.empty()) {
        return {};
    }
    
    if (word_list.find(end_word) == word_list.end()) {
        return {};
    }
    
    queue<vector<string>> ladder_queue;
    set<string> visited;
    
    vector<string> initial_ladder = {begin_word};
    ladder_queue.push(initial_ladder);
    visited.insert(begin_word);
    
    int max_iterations = 1000000;
    int iterations = 0;
    
    while (!ladder_queue.empty() && iterations < max_iterations) {
        iterations++;
        vector<string> current_ladder = ladder_queue.front();
        ladder_queue.pop();
        
        string last_word = current_ladder.back();
        
        for (const string& next_word : word_list) {
          
            if (visited.find(next_word) != visited.end()) {
                continue;
            }
            
            if (is_adjacent(last_word, next_word)) {
               
                visited.insert(next_word);
                
                vector<string> new_ladder = current_ladder;
                new_ladder.push_back(next_word);
                
                if (next_word == end_word) {
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
    
    vector<string> same_word_ladder = generate_word_ladder("were", "were", word_list);
    my_assert(same_word_ladder.size() == 1);
}