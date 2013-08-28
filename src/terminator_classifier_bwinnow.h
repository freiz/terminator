//
//  terminator_classifier_bwinnow.h
//  terminator
//
//  Created by freiz on 13-8-28.
//  Copyright (c) 2013年 freiz. All rights reserved.
//

#ifndef terminator_terminator_classifier_bwinnow_h
#define terminator_terminator_classifier_bwinnow_h

#include "terminator_classifier_base.h"

class TerminatorClassifierBWinnow: public TerminatorClassifierBase
{
private:
  static const double DEFAULT_BWINNOW_ALPHA;
  static const double DEFAULT_BWINNOW_BETA;
  static const double DEFAULT_BWINNOW_SHIFT;
  static const double DEFAULT_BWINNOW_THRESHOLD;
  static const double DEFAULT_BWINNOW_THICKNESS;
  static const double DEFAULT_BWINNOW_MAX_ITERATIONS;
  
  double bwinnow_alpha_;
  double bwinnow_beta_;
  double bwinnow_shift_;
  double bwinnow_threshold_;
  double bwinnow_thickness_;
  double bwinnow_max_iterations_;

public:
  TerminatorClassifierBWinnow();
  virtual double Predict(map<string, node>&);
  virtual void Train(map<string, node>&, bool);
};


#endif
