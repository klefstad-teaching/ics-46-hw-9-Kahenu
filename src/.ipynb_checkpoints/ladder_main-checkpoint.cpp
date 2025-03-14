#include "ladder.h"

int main() {
    set<string> word_list;
    load_words(word_list, "words.txt");
    
    string start_word, end_word;
    cout << "Enter the start word: ";
    cin >> start_word;
    cout << "Enter the end word: ";
    cin >> end_word;
    
    for (char& c : start_word) {
        c = tolower(c);
    }
    
    for (char& c : end_word) {
        c = tolower(c);
    }
    
    if (start_word == end_word) {
        error(start_word, end_word, "Start and end words are the same");
    }

    if (word_list.find(end_word) == word_list.end()) {
        error(start_word, end_word, "End word is not in the dictionary");
    }
    
    vector<string> ladder = generate_word_ladder(start_word, end_word, word_list);
    
    if (ladder.empty()) {
        cout << "No word ladder found between " << start_word << " and " << end_word << "." << endl;
    } else {
        cout << "Word ladder from " << start_word << " to " << end_word << ":" << endl;
        print_word_ladder(ladder);
    }
    
    return 0;
}