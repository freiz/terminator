#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include <string>
#include <cmath>
#include <map>

#include "common.h"

using namespace std;















//const double owv_step = 0.02;



double hit_predict(map<string, node>&);
void hit_train(map<string, node>&, bool);



#endif
