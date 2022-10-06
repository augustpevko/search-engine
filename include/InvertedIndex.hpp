#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <map>
#include <mutex>
#include <exception>
#include <iostream>

#include "simple-thread-pull.hpp"

/**
 * @brief It stores document id and count of entries.
 * 
 */
struct Entry {
    size_t docId, count;
    
    bool operator ==(const Entry& other) const {
        return (docId == other.docId &&
        count == other.count);
    }
};

/**
 * @brief Class makes inverted index to get ranks.
 * 
 */
class InvertedIndex {
public:

    /**
     * @brief Construct a new Inverted Index object.
     * It calls UpdateDocumentBase method.
     * 
     * @param input_docs contains of .txt files.
     */
    InvertedIndex(const std::vector<std::string>& input_docs);

    /**
     * @brief It makes a frequency dictionary.
     * It uses multithreading.
     * Count of threads defined in CMakeLists.txt.
     * 
     * @param input_docs contains of .txt files.
     */
    void UpdateDocumentBase(const std::vector<std::string>& input_docs);

    /**
     * @brief Get entries by word.
     * 
     * @param word that is being requested. 
     * @return Vector of document id and count of entries struct.
     */
    const std::vector<Entry> GetWordEntries(const std::string& word);

private:
    /**
     * @brief Method to index words with multithreading.
     * Method is called by UpdateDocumentBase method.
     * 
     * @param docId document index.
     * @param invertedIndex pointer to current instance.
     * 
     * @throws It throws std::runtime_error if inverterIndex point
     * to nullptr.
     */
    static void IndexWords(size_t docId, InvertedIndex* invertedIndex);

    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freqDictionary;
    std::mutex mainMutex;
};