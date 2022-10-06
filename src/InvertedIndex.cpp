#include "InvertedIndex.hpp"

InvertedIndex::InvertedIndex(const std::vector<std::string>& input_docs) {
    UpdateDocumentBase(input_docs);
}

void InvertedIndex::IndexWords(size_t docId, InvertedIndex* invertedIndex) {
    if (invertedIndex == nullptr) {
        throw std::runtime_error("invertedIndex point to nullptr");
    }

    std::string txtFile = invertedIndex->docs[docId];

    std::istringstream iss(txtFile, std::istringstream::in);
    for (std::string word; iss >> word;) {
        std::transform(word.begin(), word.end(), word.begin(),
            [](unsigned char ch){ return std::tolower(ch); });
        bool find{false};
        std::lock_guard<std::mutex> lock(invertedIndex->mainMutex);
        for (auto& ent: invertedIndex->freqDictionary[word]) {
            if (ent.docId == docId) {
                ent.count++;
                find = true;
                break;
            }
        }
        if (!find) {
            invertedIndex->freqDictionary[word].push_back(Entry({docId, 1}));
        }
    }
}

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& input_docs) {
    if (input_docs.empty()) {
        std::cerr << "Docs are empty" << std::endl;
        return;
    }

    docs = input_docs;
    freqDictionary.clear();

    ThreadPool threadPool;
    for (size_t i = 0; i < docs.size(); i++) {
        threadPool.addTask(&InvertedIndex::IndexWords, i, this);
    }
    threadPool.waitAll();
}

const std::vector<Entry> InvertedIndex::GetWordEntries(const std::string& word) {
    std::lock_guard<std::mutex> lock(mainMutex);
    return freqDictionary[word];
}