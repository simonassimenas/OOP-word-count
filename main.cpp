#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <map>
#include <set>
#include <cctype>

using namespace std;

int main() {
    // ==IO==
    ifstream inputFile("input.txt");
    if (!inputFile) {
        cerr << "Error: Unable to open input file!" << endl;
        return 1;
    }

    ofstream outputFile("output.txt", ios_base::out | ios_base::binary);
	outputFile.imbue(locale("lt_LT.UTF-8"));
    if (!outputFile) {
        cerr << "Error: Unable to open output file!" << endl;
        return 1;
    }


    // ==URL==
    string text((istreambuf_iterator<char>(inputFile)),
                istreambuf_iterator<char>());

    regex url_pattern("(https?://)?(www\\.)?[a-zA-Z0-9]+\\.[a-zA-Z]+(\\.[a-zA-Z]+)?(/[a-zA-Z0-9]*)*");

    sregex_iterator it(text.begin(), text.end(), url_pattern);
    sregex_iterator end;

    set<string> urls;
    for (; it != end; ++it) {
        urls.insert(it->str());
    }


    // ==URL OUTPUT==
    outputFile << "URL's in the presented text: " << "\n";
    for (const auto& url : urls) {
        outputFile << url << "\n";
    }
    outputFile << "\n";

    inputFile.seekg(0);


    // ==WORD COUNT==
    map<string, int > wordCount;
    map<string, set<int> > wordLines;
    int lineNum = 0;
    string line;

    while (getline(inputFile, line)) {
        ++lineNum;
        stringstream ss(line);
        string word;
        
        while (ss >> word) {
            // reikia padaryti kad neimtu simboliu bet paimtu lietuviskas raides
            while (!word.empty() && !isalpha(word[0], locale("lt_LT.UTF-8"))) {
                word.erase(0, 1);
            }
            while (!word.empty() && !isalpha(word[word.length() - 1], locale("lt_LT.UTF-8"))) {
                word.erase(word.length() - 1, 1);
            }

            //transform(word.begin(), word.end(), word.begin(), ::tolower);

            wordCount[word]++;
            wordLines[word].insert(lineNum);
        }
    }


    // ==CROSS-REFERENCE TABLE OUTPUT==
    outputFile << "Cross-reference table:\n" <<
                setw(30) << fixed << left << "Word" <<
                setw(10) << fixed << left << "Count" <<
                " References\n" << string(53, '-') << "\n";

    for (const auto& word : wordCount) {
        if (word.second > 1) {
            string wordCapitalized = word.first;
            wordCapitalized[0] = toupper(wordCapitalized[0]);

            outputFile << setw(30) << fixed << left << wordCapitalized <<
                        setw(11) << fixed << left << word.second <<
                        "Lines:";

            for (int line : wordLines[word.first]) {
                outputFile << " " << line;
            }
            outputFile << "\n";
        }
    }

    return 0;
}
