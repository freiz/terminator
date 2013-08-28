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

double winnow_predict(map<string, node>& tmp_weights)
{
  double score = 0.0;
  map<string, node>::iterator iter;
  for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
  {
    score += (iter->second).winnow;
  }
  score /= tmp_weights.size();
  score -= winnow_threshold;
  score = logist(score / winnow_shift);
  return score;
}

void winnow_train(map<string, node>& tmp_weights,
                  bool email_type)
{
  map<string, node>::iterator iter;
  double score = winnow_predict(tmp_weights);

  int count = 0;
  if (email_type && score < algorithm_threshold + winnow_thickness
      && count < winnow_max_iters)
  {
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).winnow *= winnow_alpha;
    }
    count++;
  }

  count = 0;
  if (!email_type && score > algorithm_threshold - winnow_thickness
      && count < winnow_max_iters)
  {
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).winnow *= winnow_beta;
    }
    count++;
  }
}

double pa_predict(map<string, node>& tmp_weights)
{
  double score = 0.0;
  map<string, node>::iterator iter;
  for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
  {
    score += (iter->second).pa;
  }
  score = logist(score / pa_shift);
  return score;
}

void pa_train(map<string, node>& tmp_weights,
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
    score += (iter->second).pa;
  }
  // hinge loss
  double loss = 0 > (1.0 - label * score) ? 0 : (1.0 - label * score);
  double tol = loss / tmp_weights.size();
  for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
  {
    (iter->second).pa += label * tol;
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

double nsnb_predict(map<string, node>& tmp_weights)
{
  double score = 0.0;
  int s, h;
  map<string, node>::iterator iter;
  for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
  {
    s = (iter->second).nsnb_spam;
    h = (iter->second).nsnb_ham;
    if (s == 0 && h == 0)
      continue;
    score += log(
               (s + nsnb_smooth) / (h + nsnb_smooth)
               * (total_ham + 2 * nsnb_smooth)
               / (total_spam + 2 * nsnb_smooth)
               * (iter->second).nsnb_confidence);
  }
  score += log((total_spam + nsnb_smooth) / (total_ham + nsnb_smooth));
  score = logist(score / nsnb_shift);
  return score;
}

void nsnb_train_cell(map<string, node>& tmp_weights,
                     bool email_type)
{
  map<string, node>::iterator iter;
  if (email_type)
  {
    total_spam += 1;
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).nsnb_spam += 1;
    }
  }
  else
  {
    total_ham += 1;
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).nsnb_ham += 1;
    }
  }
}
void nsnb_train(map<string, node>& tmp_weights,
                bool email_type)
{
  map<string, node>::iterator iter;
  double score = nsnb_predict(tmp_weights);
  if (email_type)
  {
    total_spam += 1;
  }
  else
  {
    total_ham += 1;
  }
  int count = 0;
  while (email_type && score < algorithm_threshold + nsnb_thickness && count < nsnb_max_iters)
  {
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).nsnb_confidence /= nsnb_learning_rate;
    }
    nsnb_train_cell(tmp_weights, email_type);
    total_spam -= 1;
    score = nsnb_predict(tmp_weights);
    count++;
  }
  count = 0;
  while (!email_type && score > algorithm_threshold - nsnb_thickness && count < nsnb_max_iters)
  {
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).nsnb_confidence *= nsnb_learning_rate;
    }
    nsnb_train_cell(tmp_weights, email_type);
    total_ham -= 1;
    score = nsnb_predict(tmp_weights);
    count++;
  }
}






