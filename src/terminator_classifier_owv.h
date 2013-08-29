//
//  terminator_classifier_owv.h
//  terminator
//
//  Created by freiz on 13-8-29.
//  Copyright (c) 2013年 freiz. All rights reserved.
//

#ifndef terminator_terminator_classifier_owv_h
#define terminator_terminator_classifier_owv_h

#include "terminator_classifier_base.h"
#include "terminator_classifier_bwinnow.h"
#include "terminator_classifier_lr.h"
#include "terminator_classifier_nb.h"
#include "terminator_classifier_nsnb.h"
#include "terminator_classifier_winnow.h"
#include "terminator_classifier_pa.h"
#include "terminator_classifier_pam.h"
#include "terminator_classifier_hit.h"

#define CLASSIFIER_NUMBER (8)

class TerminatorClassifierOWV: public TerminatorClassifierBase
{
  
private:
  
  static const double DEFAULT_OWV_THRESHOLD;
  static const double DEFAULT_OWV_STEP;
  static const unsigned DEFAULT_CLASSIFIER_NUMBER;
  static const double DEFAULT_OWV_SPAM_TRADEOFF;
  
  double owv_threshold_;
  double owv_step_;
  double owv_spam_tradeoff;
  
  TerminatorClassifierBase* classifiers_[CLASSIFIER_NUMBER] = {
    new TerminatorClassifierBWinnow(),
    new TerminatorClassifierLR(),
    new TerminatorClassifierNB(),
    new TermiantorClassifierNSNB(),
    new TerminatorClassifierWinnow(),
    new TerminatorClassifierPA(),
    new TerminatorClassifierPAM(),
    new TerminatorClassifierHIT()
  };
  
  double weights_classifier_[CLASSIFIER_NUMBER];
  
public:
  TerminatorClassifierOWV(double* weights_classifier);
  virtual void Train(map<string, node>& weights, bool is_spam);
  virtual double Predict(map<string, node>& weights);
};

#endif
