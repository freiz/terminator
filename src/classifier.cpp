#include <cstdlib>
#include "classifier.h"
//#include "common.h"

double nb_predict(map<string, node>& tmp_weights)
{
  double score = 0.0;
  map<string, node>::iterator iter;
  int s, h;
  for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
  {
    s = (iter->second).nb_spam;
    h = (iter->second).nb_ham;
    if (s == 0 && h == 0)
      continue;
    score += log(
               (s + nb_smooth) / (h + nb_smooth) * (total_ham + 2 * nb_smooth)
               / (total_spam + 2 * nb_smooth));
  }
  score += log((total_spam + nb_smooth) / (total_ham + nb_smooth));
  score = logist(score / nb_shift);
  return score;
}

void nb_train_cell(map<string, node>& tmp_weights,
                   bool email_type)
{
  map<string, node>::iterator iter;
  if (email_type)
  {
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).nb_spam += nb_increasing;
    }
  }
  else
  {
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).nb_ham += nb_increasing;
    }
  }
}

void nb_train(map<string, node>& tmp_weights,
              bool email_type)
{
  map<string, node>::iterator iter;
  double score = nb_predict(tmp_weights);
  int count = 0;
  while (email_type && score < algorithm_threshold + nb_thickness && count < nb_max_iters)
  {
    nb_train_cell(tmp_weights, email_type);
    score = nb_predict(tmp_weights);
    count++;
  }
  count = 0;
  while (!email_type && score > algorithm_threshold - nb_thickness && count < nb_max_iters)
  {
    nb_train_cell(tmp_weights, email_type);
    score = nb_predict(tmp_weights);
    count++;
  }
}

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

double logist_predict(map<string, node>& tmp_weights)
{
  double logist_score = 0.0;
  map<string, node>::iterator iter =
    tmp_weights.begin();
  while (iter != tmp_weights.end())
  {
    logist_score += (iter->second).logist;
    ++iter;
  }
  logist_score = logist(logist_score / logist_shift);
  return logist_score;
}

double bwinnow_predict(map<string, node>& tmp_weights)
{
  double bwinnow_score = 0.0;
  map<string, node>::iterator iter =
    tmp_weights.begin();
  while (iter != tmp_weights.end())
  {
    bwinnow_score += (iter->second).bwinnow_upper
                     - (iter->second).bwinnow_lower;
    ++iter;
  }
  bwinnow_score /= tmp_weights.size();
  bwinnow_score -= bwinnow_threshold;
  bwinnow_score = logist(bwinnow_score / bwinnow_shift);
  return bwinnow_score;
}

void logist_train(map<string, node>& tmp_weights,
                  bool email_type)
{
  double logist_score = logist_predict(tmp_weights);
  map<string, node>::iterator iter;
  int count = 0;

  while (email_type && logist_score <= algorithm_threshold + logist_thickness
         && count < logist_max_iters)
  {
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).logist += (1.0 - logist_score)
                               * logist_learning_rate;
    }
    logist_score = logist_predict(tmp_weights);
    count++;
  }
  count = 0;
  while (!email_type && logist_score >= algorithm_threshold - logist_thickness
         && count < logist_max_iters)
  {
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).logist -= logist_score * logist_learning_rate;
    }
    logist_score = logist_predict(tmp_weights);
    count++;
  }
}

void bwinnow_train(map<string, node>& tmp_weights,
                   bool email_type)
{
  double bwinnow_score = bwinnow_predict(tmp_weights);
  map<string, node>::iterator iter;
  int count = 0;

  while (email_type && bwinnow_score <= algorithm_threshold + bwinnow_thickness
         && count < bwinnow_max_iters)
  {
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).bwinnow_upper *= bwinnow_alpha;
      (iter->second).bwinnow_lower *= bwinnow_beta;
    }
    bwinnow_score = bwinnow_predict(tmp_weights);
    count++;
  }
  count = 0;
  while (!email_type && bwinnow_score >= algorithm_threshold - bwinnow_thickness
         && count < bwinnow_max_iters)
  {
    for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
    {
      (iter->second).bwinnow_upper *= bwinnow_beta;
      (iter->second).bwinnow_lower *= bwinnow_alpha;
    }
    bwinnow_score = bwinnow_predict(tmp_weights);
    count++;
  }
}


