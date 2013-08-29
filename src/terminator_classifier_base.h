//
//  terminator_classifier_base.h
//  terminator
//
//  Created by freiz on 13-8-27.
//  Copyright (c) 2013年 freiz. All rights reserved.
//

#ifndef terminator_terminator_classifier_base_h
#define terminator_terminator_classifier_base_h

#include "terminator_common.h"

class TerminatorClassifierBase
{
public:
  static unsigned long long TotalSpam;
  static unsigned long long TotalHam;
  static const double CLASSIFIER_THRESHOLD;
  virtual void Train(std::map<std::string, node>& weights, bool is_spam) = 0;
  virtual double Predict(std::map<std::string, node>& weights) = 0;
};
#endif
