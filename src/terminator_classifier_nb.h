//
//  terminator_classifier_nb.h
//  terminator
//
//  Created by freiz on 13-8-28.
//  Copyright (c) 2013年 freiz. All rights reserved.
//

#ifndef terminator_terminator_classifier_nb_h
#define terminator_terminator_classifier_nb_h

#include "terminator_classifier_base.h"

class TerminatorClassifierNB: public TerminatorClassifierBase
{
private:
  static const double DEFAULT_NB_SHIFT;
  static const double DEFAULT_NB_SMOOTH;
  static const double DEFAULT_NB_THICKNESS;
  static const int DEFAULT_NB_INCREASING;
  static const int DEFAULT_NB_MAX_ITERATIONS;
  
  double nb_shift_;
  double nb_smooth_;
  double nb_thickness_;
  int nb_increasing_;
  int nb_max_iterations_;
  
  void TrainCell(map<string, node>&, bool);

public:
  TerminatorClassifierNB();
  virtual double Predict(map<string, node>&);
  virtual void Train(map<string, node>&, bool);

};


#endif
