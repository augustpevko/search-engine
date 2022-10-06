#include <iostream>
#include <string>

#include <ConverterJSON.hpp>
#include <InvertedIndex.hpp>
#include <SearchServer.hpp>

int main() {
    ConverterJSON converterJSON;
    InvertedIndex invertedIndex(converterJSON.GetTextDocuments());
    SearchServer searchServer(&invertedIndex);
    converterJSON.putAnswers(searchServer.search(converterJSON.GetRequests()));
}