//
//  terminator_classifier_lr.h
//  terminator
//
//  Created by freiz on 13-8-27.
//  Copyright (c) 2013年 freiz. All rights reserved.
//

#ifndef terminator_terminator_classifier_lr_h
#define terminator_terminator_classifier_lr_h

#include "terminator_common.h"

static const double logist_learning_rate = 0.01;
static const double logist_shift = 10;
static const double logist_thickness = 0.20;
static const double logist_max_iters = 200;

double logist_predict(map<string, node>&);
void logist_train(map<string, node>&, bool);

#endif
