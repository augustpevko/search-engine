#include "SearchServer.hpp"

SearchServer::SearchServer(InvertedIndex* inInvertedIndex) {
    if (inInvertedIndex == nullptr)
        throw std::runtime_error("invertedIndex point to nullptr");

    inIndex = inInvertedIndex;
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

bool SearchServer::makeStrict(std::vector<std::vector<Entry>>& entriesOfRequest,
    std::vector<size_t>& docIds,
    size_t& reqNum) const {

    for (size_t i = 0; i < docIds.size(); i++)
        docIds[i] = entriesOfRequest[0][i].docId;

    for (size_t i = 1; i < entriesOfRequest.size(); i++) {
        auto entries{entriesOfRequest[i]};
        docIds.erase(std::remove_if(docIds.begin(), docIds.end(),
            [entries](auto it) {
                bool find{false};
                for (const auto& ent: entries) {
                    if (ent.docId == it) 
                        find = true;
                }
                return !find;
            }), docIds.end());
        if (docIds.empty())
            break;
    }
    if (docIds.empty()) {
        reqNum++;
        return true;
    }
    for (size_t i = 1; i < entriesOfRequest.size(); i++) {
        auto entries{entriesOfRequest[i]};
        entries.erase(std::remove_if(entries.begin(), entries.end(),
            [docIds](auto it) {
                bool find{false};
                for (const auto& id: docIds) {
                    if (id == it.docId) {
                        find = true;
                        break;
                    }
                }
                return find;
            }), entries.end());
    }
    return false;
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

        std::sort(entriesOfRequest.begin(),
            entriesOfRequest.end(),
            [](std::vector<Entry> first, std::vector<Entry> second) {
                size_t sumFirst{0};
                for (const auto& ent: first) 
                    sumFirst += ent.count;

                size_t sumSecond{0};
                for (const auto& ent: second)
                    sumSecond += ent.count;

                return sumFirst < sumSecond;
            });
        std::vector<size_t> docIds(entriesOfRequest[0].size());
        bool isEmpty = makeStrict(entriesOfRequest, docIds, reqNum);
        if (isEmpty)
            continue;
            
        std::vector<size_t> absRank(docIds.size());
        for (const auto& id: docIds) {
            size_t sum{0};
            for (const auto& entrs: entriesOfRequest) {
                for (const auto& ent: entrs) {
                    if (ent.docId == id)
                        sum += ent.count;
                }
            }
            absRank[id] = {sum};
        }

        auto maxCount = *std::max_element(absRank.begin(), absRank.end());
        for (size_t i = 0; i != absRank.size(); i++)
            answers[reqNum].push_back({i, float(absRank[i]) / float(maxCount)});

        std::sort(answers[reqNum].begin(), answers[reqNum].end(),
            [](auto firstI, auto secondI) {
                return firstI.rank > secondI.rank;
            });
        reqNum++;
    }
    return answers;
}