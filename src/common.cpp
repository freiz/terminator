#include <cmath>
#include "common.h"

using namespace std;
using namespace kyotocabinet;

double weights[NUM_OF_CLASSIFIERS];
int total_spam = 0;
int total_ham = 0;

void vectorization(const string email_content, map<string, node>& tmp_weights)
{
  int len = email_content.length();
  for (int i = 0; i <= len - NGRAM; i++)
  {
    string feature = email_content.substr(i, NGRAM);
    if (db.get(feature.c_str(), feature.size(), (char*) buff_node, size_of_node)
        == -1)
    {
      tmp_weights[feature] = initial_value;
    }
    else
      tmp_weights[feature] = *(buff_node);
  }
}

bool terminator_init(const string db_path, size_t size_of_cache)
{
  bool sign = true;
  buff_node = new node();
  sign = init_db(db_path, size_of_cache);
  if (sign)
  {
    if (db.get("AI", 2, (char*) &ai, sizeof(struct AdditionalInformation)) == -1 )
    {
      total_spam = 0;
      total_ham = 0;
      for (int i = 0; i < NUM_OF_CLASSIFIERS; i++)
      {
        weights[i] = 1.0;
      }
    }
    else
    {
      total_spam = ai.total_spam;
      total_ham = ai.total_ham;
      for (int i = 0; i < NUM_OF_CLASSIFIERS; i++)
      {
        weights[i] = ai.weights[i];
      }
    }
  }
  return sign;
}

bool init_db(const string db_path, size_t size_of_cache)
{
  db.tune_map(size_of_cache);
  return db.open(db_path, HashDB::OWRITER | HashDB::OCREATE);
}

void terminator_exit()
{
  delete(buff_node);
  ai.total_spam = total_spam;
  ai.total_ham = total_ham;
  for (int i = 0; i < NUM_OF_CLASSIFIERS; i++)
  {
    ai.weights[i] = weights[i];
  }
  db.set("AI", 2, (char*)&ai, sizeof(struct AdditionalInformation));
  db.close();
}

void save_db(map<string, node>& tmp_weights)
{
  map<string, node>::iterator iter;
  for (iter = tmp_weights.begin(); iter != tmp_weights.end(); ++iter)
  {
    db.set(iter->first.c_str(), iter->first.size(), (char*)&iter->second, size_of_node);
  }
}

double terminator_predict(string email_content)
{
  map<string, node> tmp_weights;
  vectorization(email_content, tmp_weights);
  double final_score = 0.0;
  double total_weights = 0.0;
  for (unsigned i = 0; i < filters.num_of_classifiers; i++)
  {
    final_score += (filters.predictors[i])(tmp_weights) * weights[i];
    total_weights += weights[i];
  }
  return final_score / total_weights;
}

void terminator_train(string email_content, bool email_type)
{
  map<string, node> tmp_weights;
  vectorization(email_content, tmp_weights);
  double final_score = 0.0;
  double total_weights = 0.0;
  double scores[NUM_OF_CLASSIFIERS];
  for (unsigned i = 0; i < filters.num_of_classifiers; i++)
  {
    scores[i] = (filters.predictors[i])(tmp_weights);
    final_score += scores[i] * weights[i];
    total_weights += weights[i];
  }
  final_score /= total_weights;
  if (email_type)
  {
    if (final_score > algorithm_threshold)
    {
      for (unsigned i = 0; i < filters.num_of_classifiers; i++)
      {
        if (scores[i] <= algorithm_threshold)
          weights[i] -= owv_step;
      }
    }
    else
    {
      for (unsigned i = 0; i < filters.num_of_classifiers; i++)
      {
        if (scores[i] > algorithm_threshold)
          weights[i] += 20 * owv_step;
      }
    }
  }
  else
  {
    if (final_score > algorithm_threshold)
    {
      for (unsigned i = 0; i < filters.num_of_classifiers; i++)
      {
        if (scores[i] <= algorithm_threshold)
          weights[i] += 20 * owv_step;
      }
    }
    else
    {
      for (unsigned i = 0; i < filters.num_of_classifiers; i++)
      {
        if (scores[i] > algorithm_threshold)
          weights[i] -= owv_step;
      }
    }
  }

  for (unsigned i = 0; i < filters.num_of_classifiers; i++)
  {
    (filters.trainers[i])(tmp_weights, email_type);
  }
  save_db(tmp_weights);
}
