#include <iostream>
#include <list>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

// Structure to store candidate data
struct Candidate {
    string name;
    string skills;
    int experience;
    float score;
};

// Function to display candidate details
void display(const Candidate &c) {
    cout << "Name: " << c.name
         << " | Skills: " << c.skills
         << " | Experience: " << c.experience
         << " years | Score: " << c.score << endl;
}

// Function to input candidate data
Candidate getCandidateInput() {
    Candidate c;
    cin.ignore();
    cout << "\nEnter candidate name: ";
    getline(cin, c.name);
    cout << "Enter skills (comma separated): ";
    getline(cin, c.skills);
    cout << "Enter years of experience: ";
    cin >> c.experience;
    cout << "Enter test score (out of 100): ";
    cin >> c.score;
    return c;
}

// Function to shortlist candidates
list<Candidate> shortlistCandidates(list<Candidate> &all, int minExp, float minScore) {
    list<Candidate> shortlisted;
    for (auto &c : all) {
        if (c.experience >= minExp && c.score >= minScore)
            shortlisted.push_back(c);
    }
    return shortlisted;
}

// Function to sort shortlisted candidates by score (descending)
void sortByScore(list<Candidate> &lst) {
    lst.sort([](const Candidate &a, const Candidate &b) {
        return a.score > b.score;
    });
}

// Function to save shortlisted candidates to CSV file
void saveToCSV(list<Candidate> &lst, const string &filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    file << "Name,Skills,Experience,Score\n";
    for (auto &c : lst)
        file << c.name << "," << c.skills << "," << c.experience << "," << c.score << "\n";

    file.close();
    cout << "\nShortlisted candidates saved to " << filename << " (open with Excel)\n";
}

// Function to read CSV data using stringstream
list<Candidate> readCSV(const string &filename) {
    list<Candidate> candidates;
    ifstream file(filename);
    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string name, skills, expStr, scoreStr;
        getline(ss, name, ',');
        getline(ss, skills, ',');
        getline(ss, expStr, ',');
        getline(ss, scoreStr, ',');

        Candidate c;
        c.name = name;
        c.skills = skills;
        stringstream(expStr) >> c.experience;
        stringstream(scoreStr) >> c.score;

        candidates.push_back(c);
    }
    return candidates;
}

int main() {
    list<Candidate> allCandidates;                 // Linked list to store all candidates
    map<string, Candidate*> candidateLookup;       // Map for quick lookup by name

    int n;
    cout << "Enter number of candidates: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        cout << "\n--- Candidate " << i + 1 << " ---";
        Candidate c = getCandidateInput();
        allCandidates.push_back(c);
        candidateLookup[c.name] = &allCandidates.back(); // Store pointer for fast lookup
    }

    int minExp;
    float minScore;
    cout << "\nEnter minimum experience required: ";
    cin >> minExp;
    cout << "Enter minimum test score required: ";
    cin >> minScore;

    list<Candidate> shortlisted = shortlistCandidates(allCandidates, minExp, minScore);
    sortByScore(shortlisted);

    cout << "\n Shortlisted Candidates:\n";
    for (auto &c : shortlisted)
        display(c);

    saveToCSV(shortlisted, "shortlisted_candidates.csv");

    // Optional: show reading CSV back
    cout << "\nReading data back from CSV using stringstream:\n";
    list<Candidate> readBack = readCSV("shortlisted_candidates.csv");
    for (auto &c : readBack)
        display(c);

    // Quick lookup demonstration
    cout << "\n🔍 Search candidate by name: ";
    string searchName;
    cin.ignore();
    getline(cin, searchName);
    if (candidateLookup.find(searchName) != candidateLookup.end())
        display(*candidateLookup[searchName]);
    else
        cout << "Candidate not found!\n";

    return 0;
}
