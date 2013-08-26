#include <iostream>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <map>
#include <vector>
#include <kchashdb.h>

#include "common.h"

using namespace std;
using namespace kyotocabinet;

int main(int argc, const char** argv)
{
  clock_t begin = clock();
  terminator_init("terminator.kch", 5 << 20);
  string corpus_path = "F:/dataset/spamassassin/";

  string index_path = corpus_path + "index";
  ifstream index;
  index.open(index_path.c_str());
  string email_type, email_path;
  char buff[MAX_READ_LENGTH];
  char buff_result[200];
  vector<string> email_type_list;
  vector<string> email_path_list;
  while (index >> email_type >> email_path)
  {
    email_type_list.push_back(email_type);
    email_path_list.push_back(email_path);
  }
  int total_emails = email_path_list.size();
  int email_count = 0;

  int ss = 0;
  int sh = 0;
  int hs = 0;
  int hh = 0;
  while (email_count < total_emails)
  {
    email_type = email_type_list[email_count];
    email_path = email_path_list[email_count];
    ++email_count;
    if (email_count % 500 == 0)
    {
      clock_t now = clock();
      printf("Processing --> %5.2f%%\t\tElapsed Time --> %6.1f (s)\n",
             (email_count + 0.0) / total_emails * 100,
             (now - begin + 0.0) / CLOCKS_PER_SEC);
      printf("ham->spam =  %3d\t\tspam->ham =  %3d\n", hs, sh);
    }
    memset(buff, 0, MAX_READ_LENGTH);
    email_path = corpus_path + email_path;
    ifstream email;
    email.open(email_path.c_str());
    email.read(buff, MAX_READ_LENGTH);
    email.close();

    double score = 0.5;
    if (email_type == "ham" || email_type == "spam" || email_type == "Ham"
        || email_type == "Spam")
    {
      score = terminator_predict(buff);
      string prediction, judge;
      if (score > THRESHOLD
         )
        prediction = "spam";
      else
        prediction = "ham";
      if (email_type == "ham")
      {
        if (prediction == "ham")
          hh += 1;
        else
          hs += 1;
      }
      else
      {
        if (prediction == "ham")
          sh += 1;
        else
          ss += 1;
      }
    }
    if (email_type == "ham" || email_type == "HAM")
      terminator_train(buff, false);
    if (email_type == "spam" || email_type == "SPAM")
      terminator_train(buff, true);
  }

  index.close();

  clock_t end = clock();
  fprintf(stderr, "Time: %.2f\n", (end - begin + 0.0) / CLOCKS_PER_SEC);
  fprintf(stderr, "Average: %.1f (e-mails per second)\n",
          email_count / ((end - begin + 0.0) / CLOCKS_PER_SEC));

  printf("ham->spam =  %3d\t\tspam->ham =  %3d\n", hs, sh);

  terminator_exit();
  return EXIT_SUCCESS;
}
