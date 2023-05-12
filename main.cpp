#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <locale>
#include <map>
#include <set>

using namespace std;

int main() {
    // ==IO==
    locale::global(locale("lt_LT.UTF-8"));

    wifstream inputFile("input.txt");
    inputFile.imbue(locale());
    if (!inputFile) {
        cerr << "Error: Unable to open input file!" << endl;
        return 1;
    }

    wofstream outputFile("output.txt", ios_base::out | ios_base::binary);
    outputFile.imbue(locale());
    if (!outputFile) {
        cerr << "Error: Unable to open output file!" << endl;
        return 1;
    }


    // ==URLS==
    wstringstream textStream;
    textStream << inputFile.rdbuf();
    wstring text = textStream.str();

    wregex url_pattern(L"(https?://)?(www\\.)?[a-zA-Z0-9]+(\\.[a-zA-Z0-9]+)*\\.[a-zA-Z]+(/[a-zA-Z0-9]*)*");

    wsregex_iterator it(text.begin(), text.end(), url_pattern);
    wsregex_iterator end;

    set<wstring> urls;
    for (; it != end; ++it) {
        urls.insert(it->str());
    }
    inputFile.seekg(0);


    // ==WORD COUNT==
    map<wstring, int > wordCount;
    map<wstring, set<int> > wordLines;
    int lineNum = 0;
    wstring line;

    while (getline(inputFile, line)) {
        ++lineNum;
        wstringstream ss(line);
        wstring word;
        
        while (ss >> word) {
            while (!word.empty() && !isalpha(word[0], locale())) {
                word.erase(0, 1);
            }
            while (!word.empty() && !isalpha(word[word.length() - 1], locale())) {
                word.erase(word.length() - 1, 1);
            }

            if(!word.empty()) {
                wordCount[word]++;
            }
            wordLines[word].insert(lineNum);
        }
    }


    // ==URL OUTPUT==
    outputFile << "URL's in the presented text: \n\n";

    for (const auto& url : urls) {
        outputFile << url << "\n";
    }


    // ==CROSS-REFERENCE TABLE OUTPUT
    outputFile << "\n\nCross-reference table:\n"
                << "\n" <<
                setw(30) << fixed << left << "Word" <<
                setw(10) << fixed << left << "Count" <<
                " References\n" << wstring(27, '-') <<
                wstring(3, ' ') << wstring(8, '-') <<
                wstring(3, ' ') << wstring(60, '-') << "\n";

    for (const auto& word : wordCount) {
        if (word.second > 1) {
            wstring wordCapitalized = word.first;
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
