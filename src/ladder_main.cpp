#include "ladder.h"

int main() {
    set<string> word_list;
    load_words(word_list, "src/words.txt");

    string begin_word, end_word;
    cout << "Enter start word: ";
    cin >> begin_word;
    cout << "Enter end word: ";
    cin >> end_word;

    if (begin_word == end_word) {
        error(begin_word, end_word, "Start and end words are the same");
        return 1;
    }

    vector<string> ladder = generate_word_ladder(begin_word, end_word, word_list);
    if (ladder.empty()) {
        error(begin_word, end_word, "No ladder found");
        return 1;
    }

    print_word_ladder(ladder);
    return 0;
}