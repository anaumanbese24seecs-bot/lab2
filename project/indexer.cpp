// cpp_indexer.cpp
// Compile with: g++ -std=c++17 -O2 cpp_indexer.cpp -o cpp_indexer
// Usage: ./cpp_indexer data/steam_games.csv <text_column_index> [sample_rows]
// NOTE: this is a simple CSV reader (does NOT fully handle quoted newlines). If your CSV has complex quoting, use Python version.

#include <bits/stdc++.h>
using namespace std;

static inline vector<string> split_csv_line(const string &line) {
    // naive split by comma (does not handle commas inside quotes)
    vector<string> out;
    string cur;
    bool in_quotes = false;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            in_quotes = !in_quotes;
            continue;
        }
        if (c == ',' && !in_quotes) {
            out.push_back(cur);
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    out.push_back(cur);
    return out;
}

static inline vector<string> tokenize(const string &s) {
    vector<string> toks;
    string cur;
    for (char ch : s) {
        char c = tolower((unsigned char)ch);
        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '\'') {
            cur.push_back(c);
        } else {
            if (!cur.empty()) { toks.push_back(cur); cur.clear(); }
        }
    }
    if (!cur.empty()) toks.push_back(cur);
    return toks;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <csv-file> <text-col-index (0-based)> [sample_rows]\n";
        return 1;
    }
    string filename = argv[1];
    int text_col = stoi(argv[2]);
    long long sample_rows = -1;
    if (argc >= 4) sample_rows = stoll(argv[3]);

    ifstream fin(filename);
    if (!fin.is_open()) { cerr << "Failed to open " << filename << "\n"; return 1; }

    string header;
    if (!getline(fin, header)) { cerr << "Empty file\n"; return 1; }

    // Build lexicon and forward
    unordered_map<string, int> lexicon; lexicon.reserve(200000);
    vector<unordered_map<int,int>> forward; // doc_id -> map(term_id -> freq)
    string line;
    long long doc_id = 0;

    while (getline(fin, line)) {
        if (sample_rows >= 0 && doc_id >= sample_rows) break;
        auto cols = split_csv_line(line);
        string text = "";
        if (text_col >= 0 && text_col < (int)cols.size()) text = cols[text_col];
        auto toks = tokenize(text);
        unordered_map<string,int> freq;
        for (auto &t: toks) freq[t]++;

        // Convert to term IDs
        unordered_map<int,int> docmap;
        for (auto &p : freq) {
            const string &term = p.first;
            int f = p.second;
            auto it = lexicon.find(term);
            int tid;
            if (it == lexicon.end()) {
                tid = (int)lexicon.size() + 1; // start from 1
                lexicon.emplace(term, tid);
            } else tid = it->second;
            docmap[tid] = f;
        }
        forward.push_back(move(docmap));
        ++doc_id;
    }
    fin.close();

    // Build inverted index
    unordered_map<int, vector<pair<int,int>>> inverted; // term_id -> list of (doc_id, freq)
    for (size_t d = 0; d < forward.size(); ++d) {
        for (auto &p : forward[d]) {
            inverted[p.first].emplace_back((int)d, p.second);
        }
    }

    // Save lexicon (term -> id)
    {
        ofstream fout("cpp_lexicon.txt");
        for (auto &kv : lexicon) {
            fout << kv.first << "\t" << kv.second << "\n";
        }
    }
    // Save forward (doc_id -> term_id:freq,...)
    {
        ofstream fout("cpp_forward.txt");
        for (size_t d = 0; d < forward.size(); ++d) {
            fout << d;
            for (auto &p : forward[d]) {
                fout << "\t" << p.first << ":" << p.second;
            }
            fout << "\n";
        }
    }
    // Save inverted (term_id -> doc_id:freq,...)
    {
        ofstream fout("cpp_inverted.txt");
        for (auto &kv : inverted) {
            fout << kv.first;
            for (auto &pr : kv.second) fout << "\t" << pr.first << ":" << pr.second;
            fout << "\n";
        }
    }

    cout << "Wrote cpp_lexicon.txt, cpp_forward.txt, cpp_inverted.txt\n";
    return 0;
}


