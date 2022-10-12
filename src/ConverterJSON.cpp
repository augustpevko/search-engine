#include "ConverterJSON.hpp"
#include "SearchServer.hpp"

ConverterJSON::ConverterJSON() {
    std::ifstream configFile("config.json");
    if (!configFile.is_open()) 
        throw std::runtime_error("config file is missing.");       
    if (configFile.peek() == EOF) 
        throw std::runtime_error("config file is empty.");

    nlohmann::json configJSON;
    configFile >> configJSON;
    configFile.close();

    auto versionOfProject = configJSON["config"]["version"].get<std::string>();
    if (versionOfProject != VERSION)
        throw std::runtime_error("config.json has incorrect file version");
    
    std::cout << configJSON["config"]["name"].get<std::string>() << " has just launched." << std::endl;
    std::cout << "Version: " << versionOfProject << std::endl;
}

std::vector<std::string> ConverterJSON::GetRequests() const {
    std::ifstream requestsFile("requests.json");
    if (!requestsFile.is_open()) 
        throw std::runtime_error("request file is missing.");
    if (requestsFile.peek() == EOF) 
        throw std::runtime_error("request file is empty.");
    
    nlohmann::json requestsJSON;
    requestsFile >> requestsJSON;
    requestsFile.close();

    return requestsJSON["requests"].get<std::vector<std::string>>();
}

uint32_t ConverterJSON::GetResponsesLimit() const {
    std::ifstream configFile("config.json");
    if (!configFile.is_open()) 
        throw std::runtime_error("config file is missing.");       
    if (configFile.peek() == EOF) 
        throw std::runtime_error("config file is empty.");
    
    nlohmann::json configJSON;
    configFile >> configJSON;
    configFile.close();
    return configJSON["config"]["max_responses"].get<uint32_t>();
}

void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>> answers) const {
    nlohmann::json answersJSON;
    for (size_t i = 0; const auto& doc: answers) {
        bool findResult = !(doc.empty());
        auto respMax = GetResponsesLimit();
        auto answersArray = nlohmann::json::array();
        for (size_t respCount{0}; respCount != respMax && respCount != doc.size(); respCount++) {
            auto answersPair = nlohmann::json::object();
            answersPair["docid"] = doc[respCount].docId;
            answersPair["rank"] = doc[respCount].rank;
            answersArray.push_back(answersPair);
        }
        answersJSON["answers"]["request" + std::to_string(i)]["result"] = findResult;
        answersJSON["answers"]["request" + std::to_string(i)]["relevance"] = answersArray;
        i++;
    }

    std::ofstream answerFile("answers.json");
    if (!answerFile)
        throw std::runtime_error("answers file is not available.");
    answerFile << std::setw(4) << answersJSON;
    answerFile.close();
    std::cout << "Done." << std::endl;
}

std::vector<std::string> ConverterJSON::GetTextDocuments() const {
    std::ifstream configFile("config.json");
    if (!configFile.is_open()) 
        throw std::runtime_error("config file is missing.");       
    if (configFile.peek() == EOF) 
        throw std::runtime_error("config file is empty.");
    
    nlohmann::json configJSON;
    configFile >> configJSON;
    configFile.close();
    std::vector<std::string> txtContents;
    for (const auto& fn: configJSON["files"].get<std::vector<std::string>>()) {
        std::ifstream txtFile(fn);
        txtFile.seekg(0, std::ios::end);
        auto size {txtFile.tellg()};
        std::string contentsTxtFile(size, ' ');
        txtFile.seekg(0, std::ios::beg);
        txtFile.read(&contentsTxtFile[0], size);
        txtFile.close();

        txtContents.push_back(contentsTxtFile);
    }
    return txtContents;
}