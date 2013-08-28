#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include <string>
#include <cmath>
#include <map>

#include "common.h"

using namespace std;

static const double thickness = 0.25;



static const double nsnb_shift = 3200;
static const double nsnb_smooth = 1e-5;
static const double nsnb_thickness = 0.25;
static const double nsnb_learning_rate = 0.65;
static const double nsnb_max_iters = 250;

static const double winnow_threshold = 1.0;
static const double winnow_shift = 1;
static const double winnow_thickness = 0.1;
static const double winnow_alpha = 1.23;
static const double winnow_beta = 0.83;
static const int winnow_max_iters = 20;

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




double nsnb_predict(map<string, node>&);
void nsnb_train_cell(map<string, node>&, bool);
void nsnb_train(map<string, node>&, bool);

double pam_predict(map<string, node>&);
void pam_train(map<string, node>&, bool);

double pa_predict(map<string, node>&);
void pa_train(map<string, node>&, bool);

double winnow_predict(map<string, node>&);
void winnow_train(map<string, node>&, bool);

double hit_predict(map<string, node>&);
void hit_train(map<string, node>&, bool);



#endif
