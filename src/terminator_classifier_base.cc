//
//  terminator_classifier_base.cc
//  terminator
//
//  Created by freiz on 13-8-28.
//  Copyright (c) 2013年 freiz. All rights reserved.
//

#include "terminator_classifier_base.h"

unsigned long long TerminatorClassifierBase::TotalSpam;
unsigned long long TerminatorClassifierBase::TotalHam;
const double TerminatorClassifierBase::CLASSIFIER_THRESHOLD = 0.5;