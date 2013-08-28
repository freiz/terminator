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



static const double pa_shift = 1.0;

static const double pam_shift = 1.25;
static const double pam_lambda = 0.1;
static const int pam_max_iters = 200;


const double owv_step = 0.02;






double pam_predict(map<string, node>&);
void pam_train(map<string, node>&, bool);

double pa_predict(map<string, node>&);
void pa_train(map<string, node>&, bool);


double hit_predict(map<string, node>&);
void hit_train(map<string, node>&, bool);



#endif
