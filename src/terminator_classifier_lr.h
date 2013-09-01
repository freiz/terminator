//
//  terminator_classifier_lr.h
//  terminator
//
//  Created by freiz on 13-8-27.
//  Copyright (c) 2013年 freiz. All rights reserved.
//

#ifndef terminator_terminator_classifier_lr_h
#define terminator_terminator_classifier_lr_h

#include "terminator_classifier_base.h"

class TerminatorClassifierLR: public TerminatorClassifierBase
{
private:
  static const double DEFAULT_LOGISTIC_LEARNING_RATE;
  static const double DEFAULT_LOGISTIC_SHIFT;
  static const double DEFAULT_LOGISTIC_THICKNESS;
  static const double DEFAULT_LOGISTIC_MAX_ITERATIONS;

  double logistic_learning_rate_;
  double logistic_shift_;
  double logistic_thickness_;
  double logistic_max_iterations_;

public:
  TerminatorClassifierLR();
  virtual void Train(std::map<std::string, node>& weights, bool is_spam);
  virtual double Predict(std::map<std::string, node>& weights);

};


#endif
