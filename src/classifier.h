#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include <string>
#include <cmath>
#include <map>

#include "common.h"

using namespace std;

static const double thickness = 0.25;







static const double hit_rate = 0.01;
static const double hit_shift = 60;
static const double hit_thickness = 0.27;
static const double hit_smooth = 1e-5;
static const int hit_max_iters = 250;







const double owv_step = 0.02;






double pam_predict(map<string, node>&);
void pam_train(map<string, node>&, bool);




double hit_predict(map<string, node>&);
void hit_train(map<string, node>&, bool);



#endif
