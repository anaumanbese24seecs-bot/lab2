#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

// Parse a CSV line correctly handling quotes
vector<string> parseCSVLine(const string &line) {
    vector<string> result;
    string token;
    bool inQuotes = false;

    for (char c : line) {
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            result.push_back(token);
            token.clear();
        } else {
            token += c;
        }
    }
    result.push_back(token);
    return result;
}

// Clean and lowercase a word
string cleanWord(const string &word) {
    string cleaned;
    cleaned.reserve(word.size());
    for (char c : word) {
        if (isalnum(c)) cleaned += tolower(c);
    }
    return cleaned;
}

int main() {
    // Updated input and output paths
    string inputFile = "E:\\Semester03\\DSA\\Project\\CppPrograms\\CSV_Generator\\applications.csv";
    string outputPath = "E:\\Semester03\\DSA\\Project\\CppPrograms\\CSV_Generator\\GeneratedCSVs\\";

    ifstream fin(inputFile);
    if (!fin.is_open()) {
        cerr << "Failed to open input file: " << inputFile << endl;
        return 1;
    }

    unordered_map<string, int> lexicon;           // word -> ID
    unordered_map<int, vector<int>> forwardIndex; // docID -> wordIDs
    unordered_map<int, set<int>> invertedIndex;   // wordID -> docIDs

    // Count total lines first for progress display
    size_t totalDocs = 0;
    string tempLine;
    getline(fin, tempLine); // skip header
    while (getline(fin, tempLine)) totalDocs++;
    fin.clear();
    fin.seekg(0);
    getline(fin, tempLine); // skip header again

    int wordIDCounter = 1;
    int docID = 1;

    cout << "Processing CSV..." << endl;

    while (getline(fin, tempLine)) {
        vector<string> columns = parseCSVLine(tempLine);
        if (columns.size() < 8) continue;

        string text = columns[1] + " " + columns[7]; // name + short_description

        istringstream iss(text);
        string w;
        vector<int> docWordIDs;
        while (iss >> w) {
            string clean = cleanWord(w);
            if (clean.empty()) continue;

            int wid;
            auto it = lexicon.find(clean);
            if (it == lexicon.end()) {
                wid = wordIDCounter++;
                lexicon[clean] = wid;
            } else {
                wid = it->second;
            }

            docWordIDs.push_back(wid);
            invertedIndex[wid].insert(docID);
        }

        forwardIndex[docID++] = move(docWordIDs);

        // PowerShell-friendly progress: show every 100 documents
        if ((docID-1) % 100 == 0)
            cout << "Processed documents: " << docID-1 << " / " << totalDocs << endl;
    }
    fin.close();
    cout << "Finished processing " << docID-1 << " documents." << endl;

    // Write Lexicon.csv
    ofstream lexFile(outputPath + "Lexicon.csv");
    lexFile << "Word,ID\n";
    vector<pair<string,int>> lexList(lexicon.begin(), lexicon.end());
    sort(lexList.begin(), lexList.end(), [](auto &a, auto &b){ return a.second < b.second; });
    for (auto &p : lexList) lexFile << p.first << "," << p.second << "\n";
    lexFile.close();

    // Write ForwardIndex.csv
    ofstream fwdFile(outputPath + "ForwardIndex.csv");
    fwdFile << "DocID,WordIDs\n";
    for (auto &p : forwardIndex) {
        fwdFile << p.first << ",";
        for (size_t i = 0; i < p.second.size(); i++)
            fwdFile << p.second[i] << (i+1 < p.second.size() ? " " : "");
        fwdFile << "\n";
    }
    fwdFile.close();

    // Write InvertedIndex.csv
    ofstream invFile(outputPath + "InvertedIndex.csv");
    invFile << "WordID,DocIDs\n";
    for (auto &p : invertedIndex) {
        invFile << p.first << ",";
        size_t count = 0;
        for (int d : p.second) {
            invFile << d;
            if (++count < p.second.size()) invFile << " ";
        }
        invFile << "\n";
    }
    invFile.close();

    cout << "CSV files generated successfully in " << outputPath << endl;
    return 0;
}
