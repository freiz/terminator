//
//  terminator_classifier_owv.cc
//  terminator
//
//  Created by freiz on 13-8-29.
//  Copyright (c) 2013年 freiz. All rights reserved.
//

#include "terminator_classifier_owv.h"

const double TerminatorClassifierOWV::DEFAULT_OWV_THRESHOLD = 0.5;
const double TerminatorClassifierOWV::DEFAULT_OWV_STEP = 0.02;

TerminatorClassifierOWV::TerminatorClassifierOWV(double* weights_classifier)
{
  this->owv_threshold_ = TerminatorClassifierOWV::DEFAULT_OWV_THRESHOLD;
  this->owv_step_ = TerminatorClassifierOWV::DEFAULT_OWV_STEP;
  unsigned i = 0;
  while (i++ < CLASSIFIER_NUMBER) this->weights_classifier_[i] = weights_classifier[i];
}

double TerminatorClassifierOWV::Predict(map<string, node>& weights)
{
  double final_score = 0.0;
  double total_weights = 0.0;
  for (unsigned i = 0; i < CLASSIFIER_NUMBER; i++)
  {
    final_score += classifiers_[i]->Predict(weights) * weights_classifier_[i];
    total_weights += weights_classifier_[i];
  }
  return final_score / total_weights;
}

void TerminatorClassifierOWV::Train(map<string, node>& weights, bool is_spam)
{
  double final_score = 0.0;
  double total_weights = 0.0;
  double scores[CLASSIFIER_NUMBER];
  for (unsigned i = 0; i < CLASSIFIER_NUMBER; i++)
  {
    scores[i] = classifiers_[i]->Predict(weights);
    final_score += scores[i] * weights_classifier_[i];
    total_weights += weights_classifier_[i];
  }
  final_score /= total_weights;
  if (is_spam)
  {
    if (final_score > owv_threshold_)
    {
      for (unsigned i = 0; i < CLASSIFIER_NUMBER; i++)
      {
        if (scores[i] <= owv_threshold_)
          weights_classifier_[i] -= owv_step_;
      }
    }
    else
    {
      for (unsigned i = 0; i < CLASSIFIER_NUMBER; i++)
      {
        if (scores[i] > owv_threshold_)
          weights_classifier_[i] += owv_spam_tradeoff * owv_step_;
      }
    }
  }
  else
  {
    if (final_score > owv_threshold_)
    {
      for (unsigned i = 0; i < CLASSIFIER_NUMBER; i++)
      {
        if (scores[i] <= algorithm_threshold)
          weights_classifier_[i] += owv_spam_tradeoff * owv_step_;
      }
    }
    else
    {
      for (unsigned i = 0; i < CLASSIFIER_NUMBER; i++)
      {
        if (scores[i] > algorithm_threshold)
          weights_classifier_[i] -= owv_step_;
      }
    }
  }
  
  for (unsigned i = 0; i < CLASSIFIER_NUMBER; i++)
  {
    classifiers_[i]->Train(weights, is_spam);
  }
}

