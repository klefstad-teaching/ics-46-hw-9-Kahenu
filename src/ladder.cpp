#include "ladder.h"

void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg << " (" << word1 << ", " << word2 << ")" << endl;
    exit(1);
}

bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    if (str1 == str2) return true;
    
    int len1 = str1.length();
    int len2 = str2.length();
    
    if (abs(len1 - len2) > d) return false;
    
    if (d == 1) {
        if (len1 == len2) {
            int differences = 0;
            for (size_t i = 0; i < len1; ++i) {
                if (str1[i] != str2[i]) {
                    ++differences;
                    if (differences > d) return false;
                }
            }
            return true;
        } else {
            const string& shorter = (len1 < len2) ? str1 : str2;
            const string& longer = (len1 < len2) ? str2 : str1;
            
            int shorterLen = shorter.length();
            int longerLen = longer.length();
            
            if (longerLen - shorterLen > d) return false;
            
            int i = 0, j = 0;
            bool skipped = false;
            
            while (i < shorterLen && j < longerLen) {
                if (shorter[i] != longer[j]) {
                    if (skipped) return false;
                    skipped = true;
                    j++;
                } else {
                    i++;
                    j++;
                }
            }
            
            return (i == shorterLen);
        }
    }
    
    vector<int> prev(len2 + 1), curr(len2 + 1);
    
    for (int j = 0; j <= len2; j++) prev[j] = j;
    
    for (int i = 1; i <= len1; i++) {
        curr[0] = i;
        for (int j = 1; j <= len2; j++) {
            if (str1[i-1] == str2[j-1]) curr[j] = prev[j-1];
            else curr[j] = 1 + min(prev[j-1], min(prev[j], curr[j-1]));
        }
        prev = curr;
    }
    
    return prev[len2] <= d;
}

bool is_adjacent(const string& word1, const string& word2) {
    if (word1 == word2) return true;
    
    int len1 = word1.length();
    int len2 = word2.length();
    int diff = abs(len1 - len2);
    
    if (diff > 1) return false;
    
    if (len1 == len2) {
        int differences = 0;
        for (int i = 0; i < len1; i++) {
            if (word1[i] != word2[i]) {
                differences++;
                if (differences > 1) return false;
            }
        }
        return differences <= 1;
    }
    
    const string& shorter = len1 < len2 ? word1 : word2;
    const string& longer = len1 < len2 ? word2 : word1;
    
    int i = 0, j = 0;
    bool skipped = false;
    
    while (i < shorter.length() && j < longer.length()) {
        if (shorter[i] != longer[j]) {
            if (skipped) return false;
            skipped = true;
            j++;
        } else {
            i++;
            j++;
        }
    }
    
    return true;
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (begin_word == end_word) {
        return {}; 
    }
    
    unordered_set<string> fast_word_set(word_list.begin(), word_list.end());
    
    if (end_word != begin_word && fast_word_set.find(end_word) == fast_word_set.end()) {
        return {};
    }
    
    queue<vector<string>> ladder_queue;
    unordered_set<string> visited;
    
    vector<string> initial_ladder = {begin_word};
    ladder_queue.push(initial_ladder);
    visited.insert(begin_word);
    
    unordered_map<int, vector<string>> length_grouped_words;
    for (const string& word : fast_word_set) {
        length_grouped_words[word.length()].push_back(word);
    }
    
    int max_queue_size = 100000;
    
    while (!ladder_queue.empty() && ladder_queue.size() < max_queue_size) {
        vector<string> current_ladder = ladder_queue.front();
        ladder_queue.pop();
        
        string last_word = current_ladder.back();
        
        int last_word_len = last_word.length();
        
        for (int length = max(1, last_word_len - 1); length <= last_word_len + 1; length++) {
            const auto& words_of_length = length_grouped_words[length];
            for (const string& word : words_of_length) {
                if (visited.find(word) != visited.end()) {
                    continue;
                }
                
                if (is_adjacent(last_word, word)) {
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
    }
    
    return {};
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream in(file_name);
    if (!in) {
        cerr << "Error: Cannot open file " << file_name << endl;
        exit(1);
    }
    
    word_list.clear();
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
    load_words(word_list, "words.txt");
    
    #define my_assert(e) {cout << #e << ((e) ? " passed" : " failed") << endl;}
    
    vector<string> ladder1 = generate_word_ladder("cat", "dog", word_list);
    my_assert(ladder1.size() == 4);
    
    vector<string> ladder2 = generate_word_ladder("marty", "curls", word_list);
    my_assert(ladder2.size() == 6);
    
    vector<string> ladder3 = generate_word_ladder("code", "data", word_list);
    my_assert(ladder3.size() == 6);
    
    vector<string> ladder4 = generate_word_ladder("work", "play", word_list);
    my_assert(ladder4.size() == 6);
    
    vector<string> ladder5 = generate_word_ladder("sleep", "awake", word_list);
    my_assert(ladder5.size() == 8);
    
    vector<string> ladder6 = generate_word_ladder("car", "cheat", word_list);
    my_assert(ladder6.size() == 4);
    
    vector<string> ladder7 = generate_word_ladder("were", "were", word_list);
    my_assert(ladder7.size() == 0);
}