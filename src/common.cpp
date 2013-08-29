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


