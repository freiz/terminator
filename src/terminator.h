//
//  terminator.h
//  terminator
//
//  Created by freiz on 13-8-30.
//  Copyright (c) 2013年 freiz. All rights reserved.
//

#ifndef terminator_terminator_h
#define terminator_terminator_h

#include <map>
#include <string>
#include <cmath>
#include <kchashdb.h>

#include "terminator_classifier_owv.h"

#define MAX_READ_LEN (3000)
#define NGRAM (4)

class Terminator
{
private:
  static const node DEFALULT_NODE_VALUE;
  static const size_t NODE_SIZE;
  static const std::string IDENTIFIER_CLASSIFIER_WEIGHTS;
  static const std::string IDENTIFIER_TOTAL_SPAM;
  static const std::string IDENTIFIER_TOTAL_HAM;
  
  std::string db_path_;
  size_t mem_cache;
  kyotocabinet::HashDB db_;
  TerminatorClassifierBase* classifier_;
  ptr_node cache_node_;
  double classifier_weights_[CLASSIFIER_NUMBER];
  
  bool InitDB(std::string db_path, size_t mem_cache);
  bool PrepareMetaData();
  void Vectorization(std::string email_content, std::map<std::string, node>& weights);
public:
  Terminator(std::string db_path, size_t mem_cache);
  double Predict(std::string email_content);
  void Train(std::string email_content, bool is_spam);
};


#endif
