#include <cstdlib>
#include "classifier.h"
//#include "common.h"



double hit_predict(map<string, node>& tmp_weights)
{
  double score = 0.0;
  map<string, node>::iterator iter;
  for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
  {
    score += (iter->second).hit;
  }
  score = logist(score / hit_shift);
  return score;
}

void hit_train(map<string, node>& tmp_weights,
               bool email_type)
{
  map<string, node>::iterator iter;
  if (email_type)
  {
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).hit_spam += 1;
    }
  }
  else
  {
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).hit_ham += 1;
    }
  }
  double score = 0.0;
  double ratio, p;
  score = hit_predict(tmp_weights);
  int count = 0;
  while (email_type && score < algorithm_threshold + hit_thickness && count < hit_max_iters)
  {
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      p = ((iter->second).hit_spam + hit_smooth)
          / ((iter->second).hit_ham + (iter->second).hit_spam
             + 2.0 * hit_smooth);
      ratio = fabs(2 * p - 1.0);
      (iter->second).hit += (1.0 - score) * hit_rate;
      (iter->second).hit *= ratio;
    }
    score = hit_predict(tmp_weights);
    count += 1;
  }
  count = 0;
  while (!email_type && score > algorithm_threshold - hit_thickness && count < hit_max_iters)
  {
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      p = ((iter->second).hit_spam + hit_smooth)
          / ((iter->second).hit_ham + (iter->second).hit_spam
             + 2.0 * hit_smooth);
      ratio = fabs(2 * p - 1.0);
      (iter->second).hit -= score * hit_rate;
      (iter->second).hit *= ratio;
    }
    score = hit_predict(tmp_weights);
    count += 1;
  }
}




double pam_predict(map<string, node>& tmp_weights)
{
  double score = 0.0;
  map<string, node>::iterator iter;
  for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
  {
    score += (iter->second).pam;
  }
  score = logist(score / pam_shift);
  return score;
}

void pam_train(map<string, node>& tmp_weights,
               bool email_type)
{
  int label;
  if (email_type)
    label = 1;
  else
    label = -1;
  double score = 0.0;
  map<string, node>::iterator iter;
  for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
  {
    score += (iter->second).pam;
  }
  // hinge loss
  int count = 0;
  while (label * score < 1.0 && count < pam_max_iters)
  {
    double tol = pam_lambda / tmp_weights.size();
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).pam += label * tol;
    }
    score = 0.0;
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      score += (iter->second).pam;
    }
    count++;
  }
}







