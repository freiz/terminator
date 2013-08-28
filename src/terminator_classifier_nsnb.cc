//
//  terminator_classifier_nsnb.cc
//  terminator
//
//  Created by freiz on 13-8-28.
//  Copyright (c) 2013年 freiz. All rights reserved.
//

#include "terminator_classifier_nsnb.h"

const double DEFAULT_NSNB_SHIFT = 3200;
const double DEFAULT_NSNB_SMOOTH = 1e-5;
const double DEFAULT_NSNB_THICKNESS = 0.25;
const double DEFAULT_NSNB_LEARNING_RATE = 0.65;
const double DEFAULT_NSNB_MAX_ITERATIONS = 250;

TermiantorClassifierNSNB::TermiantorClassifierNSNB()
{
  this->nsnb_shift_ = TermiantorClassifierNSNB::DEFAULT_NSNB_SHIFT;
  this->nsnb_smooth_ = TermiantorClassifierNSNB::DEFAULT_NSNB_SMOOTH;
  this->nsnb_thickness_ = TermiantorClassifierNSNB::DEFAULT_NSNB_THICKNESS;
  this->nsnb_learning_rate_ = TermiantorClassifierNSNB::DEFAULT_NSNB_LEARNING_RATE;
  this->nsnb_max_iterations_ = TermiantorClassifierNSNB::DEFAULT_NSNB_MAX_ITERATIONS;
}

double TermiantorClassifierNSNB::Predict(map<string, node>& tmp_weights)
{
  double score = 0.0;
  int s, h;
  map<string, node>::iterator iter;
  for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
  {
    s = (iter->second).nsnb_spam;
    h = (iter->second).nsnb_ham;
    if (s == 0 && h == 0)
      continue;
    score += log(
                 (s + this->nsnb_smooth_) / (h + this->nsnb_smooth_)
                 * (TerminatorClassifierBase::TotalHam + 2 * this->nsnb_smooth_)
                 / (TerminatorClassifierBase::TotalSpam + 2 * this->nsnb_smooth_)
                 * (iter->second).nsnb_confidence);
  }
  score += log((TerminatorClassifierBase::TotalSpam + this->nsnb_smooth_) / (TerminatorClassifierBase::TotalHam + this->nsnb_smooth_));
  score = logist(score / this->nsnb_shift_);
  return score;
}

void TermiantorClassifierNSNB::TrainCell(map<string, node>& tmp_weights, bool email_type)
{
  map<string, node>::iterator iter;
  if (email_type)
  {
    TerminatorClassifierBase::TotalSpam += 1;
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).nsnb_spam += 1;
    }
  }
  else
  {
    TerminatorClassifierBase::TotalHam += 1;
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).nsnb_ham += 1;
    }
  }
}
void TermiantorClassifierNSNB::Train(map<string, node>& tmp_weights,
                bool email_type)
{
  map<string, node>::iterator iter;
  double score = this->Predict(tmp_weights);
  if (email_type)
  {
    TerminatorClassifierBase::TotalSpam += 1;
  }
  else
  {
    TerminatorClassifierBase::TotalHam += 1;
  }
  int count = 0;
  while (email_type && score < algorithm_threshold + this->nsnb_thickness_ && count < this->nsnb_max_iterations_)
  {
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).nsnb_confidence /= this->nsnb_learning_rate_;
    }
    TrainCell(tmp_weights, email_type);
    TerminatorClassifierBase::TotalSpam -= 1;
    score = this->Predict(tmp_weights);
    count++;
  }
  count = 0;
  while (!email_type && score > algorithm_threshold - this->nsnb_thickness_ && count < this->nsnb_max_iterations_)
  {
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).nsnb_confidence *= this->nsnb_learning_rate_;
    }
    TrainCell(tmp_weights, email_type);
    TerminatorClassifierBase::TotalHam -= 1;
    score = this->Predict(tmp_weights);
    count++;
  }
}
