#pragma once

#include <vector>
#include <string>
#include <exception>
#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>

#include "nlohmann/json.hpp"

struct RelativeIndex;

/**
 * @brief Class for handle JSON files.
 * 
 */
class ConverterJSON {
public:
    
    /**
     * @brief Construct a new Converter JSON object
     * It checks config.json file.
     * It compares version of application and version of config.json.
     * It prints message to console about name and version.
     * 
     * @throws It throws std::runtime_error
     * if config file is missing,
     * if config file is empty,
     * if version of application and version of config.json don't equal.
     * 
     */
    ConverterJSON();

    ConverterJSON(const ConverterJSON &) = delete;
    ConverterJSON(ConverterJSON &&) = delete;

    ConverterJSON& operator=(const ConverterJSON &) = delete;
    ConverterJSON& operator=(ConverterJSON &&) = delete;

    /**
     * @brief Method of getting contents of .txt files defined in config.json.
     * 
     * @return Contents of .txt files.
     * 
     */
    std::vector<std::string> GetTextDocuments() const;

    /**
     * @brief Get the Responses Limit count from config.json.
     * 
     * @return Responses Limit count from config.json 
     */
    uint32_t GetResponsesLimit() const;

    /**
     * @brief Get the Requests words from requests.json.
     * 
     * @return Requests words from requests.json.
     * 
     * @throws It throws std::runtime_error
     * if request file is missing,
     * if request file is empty.
     */
    std::vector<std::string> GetRequests() const;
    
    /**
     * @brief Put the Answers to answers.json.
     * 
     * @param answers the docId is id of document. The rank is rank of relevance.
     * 
     * @throws It throws std::runtime_error if answer.json is not available. 
     */
    void putAnswers(std::vector<std::vector<RelativeIndex>> answers) const;
};