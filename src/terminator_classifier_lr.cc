//
//  terminator_classifier_lr.cc
//  terminator
//
//  Created by freiz on 13-8-27.
//  Copyright (c) 2013年 freiz. All rights reserved.
//

#include "terminator_classifier_lr.h"

void logist_train(map<string, node>& tmp_weights,
                  bool email_type)
{
  double logist_score = logist_predict(tmp_weights);
  map<string, node>::iterator iter;
  int count = 0;
  
  while (email_type && logist_score <= algorithm_threshold + logist_thickness
         && count < logist_max_iters)
  {
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).logist += (1.0 - logist_score)
      * logist_learning_rate;
    }
    logist_score = logist_predict(tmp_weights);
    count++;
  }
  count = 0;
  while (!email_type && logist_score >= algorithm_threshold - logist_thickness
         && count < logist_max_iters)
  {
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).logist -= logist_score * logist_learning_rate;
    }
    logist_score = logist_predict(tmp_weights);
    count++;
  }
}

double logist_predict(map<string, node>& tmp_weights)
{
  double logist_score = 0.0;
  map<string, node>::iterator iter =
  tmp_weights.begin();
  while (iter != tmp_weights.end())
  {
    logist_score += (iter->second).logist;
    ++iter;
  }
  logist_score = logist(logist_score / logist_shift);
  return logist_score;
}