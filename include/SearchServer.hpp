#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <set>
#include <exception>

#include "InvertedIndex.hpp"

class InvertedIndex;


/**
 * @brief It stores document id and relative rank.
 * 
 */
struct RelativeIndex {
    size_t docId;
    float rank;
    bool operator ==(const RelativeIndex& other) const {
        return (docId == other.docId && rank == other.rank);
    }
};

/**
 * @brief Class for searching words from requests.
 * 
 */
class SearchServer {
public:
    /**
     * @brief Construct a new Search Server object.
     * 
     * @param idx pointer to InvertedIndex object to get frequency
     * of words.
     * 
     * @throws It throws std::runtime_error if invertedIndex point
     * to nullptr.
     */
    SearchServer(InvertedIndex* inInvertedIndex);
   
    /**
     * @brief Method of processing search queries.
     * 
     * @param queriesInput requests from calling GetRequests method.
     * @return answers are entered by calling the putAnswers method.
     * index of vector is id of .txt file. Internal vector sorted by rank. 
     */
    const std::vector<std::vector<RelativeIndex>> search(const
        std::vector<std::string>& queriesInput);
private:
    /**
     * @brief To get unique words from string of words.
     * 
     * @param request words.
     * @return unique words with lowercase letters. 
     */
    const std::set<std::string> getUniqueWords(const std::string& request);

    /**
     * @brief relevance is calculated only if all the words from the query exist in the document.
     * This is if you would used quotation marks for each word in query.
     * 
     * @param entriesOfRequest entries of words in request. 
     * @param docIds vector to store only the necessary documents id.
     * @param reqNum index of request.
     * @return true if docIds is empty.
     * @return false if docIds isn't empty.
     */
    bool makeStrict(std::vector<std::vector<Entry>>& entriesOfRequest,    
                              std::vector<size_t>& docIds,
                              size_t& reqNum);
    InvertedIndex* inIndex;
};