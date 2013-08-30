//
//  terminator.cc
//  terminator
//
//  Created by freiz on 13-8-30.
//  Copyright (c) 2013年 freiz. All rights reserved.
//

#include "terminator.h"

const node Terminator::DEFALULT_NODE_VALUE =
{
  0.0,
  2.0,
  1.0,
  0,
  0,
  1.0,
  0.0,
  0.0,
  1.0,
  0,
  0,
  0.0,
  0,
  0
};

const size_t Terminator::NODE_SIZE = sizeof(node);

const std::string Terminator::IDENTIFIER_CLASSIFIER_WEIGHTS = "classifier:weights";
const std::string Terminator::IDENTIFIER_TOTAL_SPAM = "classifier:total:spam";
const std::string Terminator::IDENTIFIER_TOTAL_HAM = "classifier:total:ham";

Terminator::Terminator(std::string db_path, size_t mem_cache)
{
  
}

bool Terminator::InitDB(std::string db_path, size_t mem_cache)
{
  db_.tune_map(mem_cache);
  return db_.open(db_path_, kyotocabinet::HashDB::OWRITER | kyotocabinet::HashDB::OCREATE);
}

bool Terminator::PrepareMetaData()
{
  
}

void Terminator::Vectorization(std::string email_content, std::map<std::string, node>& weights)
{
  size_t len = email_content.length();
  len = len <= MAX_READ_LEN ? len : MAX_READ_LEN;
  for (int i = 0; i <= len - NGRAM; i++)
  {
    std::string feature = email_content.substr(i, NGRAM);
    if (db_.get(feature.c_str(), feature.size(), (char*) cache_node_, Terminator::NODE_SIZE)
        == -1)
    {
      weights[feature] = Terminator::DEFALULT_NODE_VALUE;
    }
    else
      weights[feature] = *(cache_node_);
  }
}

