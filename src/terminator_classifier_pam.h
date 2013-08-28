//
//  terminator_classifier_pam.h
//  terminator
//
//  Created by freiz on 13-8-28.
//  Copyright (c) 2013年 freiz. All rights reserved.
//

#ifndef terminator_terminator_classifier_pam_h
#define terminator_terminator_classifier_pam_h

#include "terminator_classifier_base.h"

class TerminatorClassifierPAM: public TerminatorClassifierBase
{
private:
  
  static const double DEFAULT_PAM_SHIFT;
  static const double DEFAULT_PAM_LAMBDA;
  static const int DEFAULT_PAM_MAX_ITERATIONS;
  
  double pam_shift_;
  double pam_lambda_;
  double pam_max_iterations_;
  
public:
  
  TerminatorClassifierPAM();
  virtual double Predict(map<string, node>& weights);
  virtual void Train(map<string, node>& weights, bool is_spam);
  
};

#endif
