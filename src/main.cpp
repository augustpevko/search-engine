#include <iostream>
#include <string>

#include <ConverterJSON.hpp>
#include <InvertedIndex.hpp>
#include <SearchServer.hpp>

int main() {
    ConverterJSON* converterJSON = new ConverterJSON;
    InvertedIndex* invertedIndex = new InvertedIndex(converterJSON->GetTextDocuments());
    SearchServer* searchServer = new SearchServer(invertedIndex);
    converterJSON->putAnswers(searchServer->search(converterJSON->GetRequests()));
    
    delete converterJSON;
    delete invertedIndex;
    delete searchServer;
}