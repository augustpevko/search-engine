#include "SearchServer.hpp"
#include "InvertedIndex.hpp"
#include "ConverterJSON.hpp"

SearchServer::SearchServer(InvertedIndex* inInvertedIndex, uint32_t inMaxResponses) {
    if (inInvertedIndex == nullptr)
        throw std::runtime_error("invertedIndex point to nullptr");

    inIndex = inInvertedIndex;
    
    if (inMaxResponses <= 0)
        throw std::runtime_error("invalid responses limit");
    maxResponses = inMaxResponses;
}

std::set<std::string> SearchServer::getUniqueWords(const std::string& request) const {
    std::set<std::string> uniqWords;
    std::istringstream iss(request, std::istringstream::in);
    for (std::string word; iss >> word;) {
        std::transform(word.begin(), word.end(), word.begin(),
            [](unsigned char ch){ return std::tolower(ch); });
        uniqWords.emplace(word);
    }
    return uniqWords;
}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const
    std::vector<std::string>& queriesInput) const {

    std::vector<std::vector<RelativeIndex>> answers(queriesInput.size());

    for (size_t reqNum = 0; const auto& req: queriesInput) {
        std::set<std::string> uniqWords = getUniqueWords(req);

        std::vector<std::vector<Entry>> entriesOfRequest(uniqWords.size());
        for (size_t i = 0; const auto& word: uniqWords) {
            entriesOfRequest[i] = {inIndex->GetWordEntries(word)};
            i++;
        }

        std::map<size_t, size_t> absRank;
        for (const auto& entrs: entriesOfRequest) {
            for (const auto& ent: entrs) {
                if (absRank.contains(ent.docId))
                    absRank[ent.docId] += ent.count;
                else
                    absRank[ent.docId] = ent.count;
            }
        }
        if (absRank.empty()) {
            reqNum++;
            continue;
        }
        answers[reqNum].resize(absRank.size());
        for (size_t i = 0; const auto& [key, value]: absRank) {
            answers[reqNum][i].docId = key;
            answers[reqNum][i].rank = value;
            i++;
        }
        std::stable_sort(answers[reqNum].begin(), answers[reqNum].end(),
            [](auto firstI, auto secondI) {
                return firstI.rank > secondI.rank;
            });
        if (absRank.size() > maxResponses)
            answers[reqNum].erase(answers[reqNum].begin() + maxResponses, answers[reqNum].end());
        auto maxElem = answers[reqNum][0].rank;

        for (auto& ent: answers[reqNum]) {
            ent.rank /= maxElem;
        }
        reqNum++;
    }
    return answers;
}