#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <iomanip>

#include "tokenizer.h"
#include "inverted_index.h"
#include "ranking_engine.h"
#include "query_processor.h"
#include "trie.h"

void printLine()
{
  std::cout << "------------------------------------------------------------\n";
}

void printTitle(const std::string &title)
{
  std::cout << "\n============================================================\n";
  std::cout << title << "\n";
  std::cout << "============================================================\n";
}

void printSection(const std::string &title)
{
  std::cout << "\n[" << title << "]\n";
  printLine();
}

int main()
{
  Tokenizer tokenizer;
  InvertedIndex index;
  RankingEngine ranker;
  QueryProcessor processor;
  Trie trie;

  std::vector<std::string> docs = {
      "Data Structures and Algorithms form the core of computer science",
      "advanced data structures like Segment trees and Fenwick trees",
      "graph algorithms include bfs, dfs, Dijkstra and Bellman ford",
      "dynamic programming is a powerful ALGorithmic technique",
      "competitive programming requires strong data structures knowledge.",
      "C plus plus is widely used for systems programming and performance",
      "hash maps and hash TABles provide average constant time access.",
      "binary search trees heaps and tries are fundamental data structures",
      "search engines use inverted index ranking and query processing",
      "tf idf is a ranking ALGorithm used in information retrieval",
      "autocomplete systems use trie data structures for fast prefix search",
      "large scale systems require efficient indexing and caching.",
      "algorithms and data structures are essential for coding interviews",
      "machine learning models depend on data preprocessing and algorithms",
      "operating systems use SCHeduling algorithms and memory management"};

  printTitle("INDEXRA : Search Engine Core (C++)");

  printSection("INDEXING PHASE");

  for (size_t docId = 0; docId < docs.size(); ++docId)
  {
    std::cout << "Document " << docId << ":\n";
    std::cout << "  Text   : " << docs[docId] << "\n";

    auto tokens = tokenizer.tokenize(docs[docId]);

    std::cout << "  Tokens : ";
    for (const auto &token : tokens)
      std::cout << token << " ";
    std::cout << "\n";

    index.addDocument(static_cast<int>(docId), tokens);
    for (const auto &token : tokens)
      trie.insert(token);

    printLine();
  }

  std::cout << "Indexing completed.\n";
  std::cout << "Total documents indexed: "
            << index.getTotalDocuments() << "\n";

  printSection("QUERY 1 : \"data structures\"");

  std::string query1 = "data structures";
  auto queryTokens1 = tokenizer.tokenize(query1);

  std::vector<std::unordered_map<int, int>> postingLists1;
  for (const auto &token : queryTokens1)
    postingLists1.push_back(index.search(token));

  auto andResult1 = processor.processAndQuery(postingLists1);
  auto orResult1 = processor.processOrQuery(postingLists1);

  std::unordered_map<int, int> blended1 = orResult1;
  const int AND_BOOST = 10;

  for (const auto &[docId, score] : andResult1)
    blended1[docId] += score * AND_BOOST;

  auto ranked1 = ranker.rankTFIDF(
      blended1,
      index.getTotalDocuments(),
      blended1.size(),
      5);

  std::cout << "Final ranked results:\n";
  printLine();
  std::cout << std::setw(6) << "Rank"
            << std::setw(6) << "Doc"
            << std::setw(10) << "Score"
            << "\n";
  printLine();

  int r = 1;
  for (const auto &[docId, score] : ranked1)
  {
    std::cout << std::setw(6) << r++
              << std::setw(6) << docId
              << std::setw(10) << std::fixed
              << std::setprecision(3) << score
              << "\n";
  }

  printSection("QUERY 2 : \"ranking algorithm\"");

  std::string query2 = "ranking algorithm";
  auto queryTokens2 = tokenizer.tokenize(query2);

  std::vector<std::unordered_map<int, int>> postingLists2;
  for (const auto &token : queryTokens2)
    postingLists2.push_back(index.search(token));

  auto andResult2 = processor.processAndQuery(postingLists2);
  auto orResult2 = processor.processOrQuery(postingLists2);

  std::unordered_map<int, int> blended2 = orResult2;
  for (const auto &[docId, score] : andResult2)
    blended2[docId] += score * AND_BOOST;

  auto ranked2 = ranker.rankTFIDF(
      blended2,
      index.getTotalDocuments(),
      blended2.size(),
      5);

  std::cout << "Final ranked results:\n";
  printLine();

  r = 1;
  for (const auto &[docId, score] : ranked2)
  {
    std::cout << std::setw(6) << r++
              << std::setw(6) << docId
              << std::setw(10) << std::fixed
              << std::setprecision(3) << score
              << "\n";
  }

  printSection("AUTOCOMPLETE DEMO");

  std::vector<std::string> prefixes = {
      "dat", "alg", "str", "pro", "aut"};

  for (const auto &prefix : prefixes)
  {
    std::cout << "Prefix \"" << prefix << "\":\n";
    auto suggestions = trie.autocomplete(prefix);

    if (suggestions.empty())
    {
      std::cout << "  No suggestions\n";
    }
    else
    {
      for (const auto &word : suggestions)
        std::cout << "  - " << word << "\n";
    }
    printLine();
  }

  printTitle("END OF DEMONSTRATION");
  return 0;
}
