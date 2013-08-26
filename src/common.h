#ifndef COMMEN_H_
#define COMMEN_H_

#include <cmath>
#include <map>
#include <string>
#include <kchashdb.h>

#define NUM_OF_CLASSIFIERS (8)
#define THRESHOLD (0.625)
#define MAX_READ_LENGTH (3000)
#define ACTIVE_THRESHOLD (0.118)
#define NGRAM (4)

using namespace std;
using namespace kyotocabinet;

/**
 * store
 * 1. the weights of different classifiers
 * 2. total number of spam/ham e-mails
 * These are important information, so we store it in database
 * @see commen::init()
 * @author freiz
 * */
struct AdditionalInformation
{
  double weights[NUM_OF_CLASSIFIERS];
  int total_spam;
  int total_ham;
};

/**
 * base model of terminator
 * store information of each feature
 * */
struct Node
{
  float logist;
  float bwinnow_upper;
  float bwinnow_lower;
  int nsnb_spam;
  int nsnb_ham;
  double nsnb_confidence;
  float pam;
  float pa;
  float winnow;
  int hit_spam;
  int hit_ham;
  float hit;
  int nb_spam;
  int nb_ham;
};
typedef struct Node node;
typedef struct Node * ptr_node;

/**
 * initial value of node
 * when a new feature comes, fill it with this initial value
 * */
static const node initial_value =
{
  0.0,
  2.0,
  1.0,
  0,
  0,
  1.0,
  0.0,
  0.0,
  1.0,
  0,
  0,
  0.0,
  0,
  0
};

static HashDB db;  // database, we use the hash file db of kyoto cabinet
static ptr_node buff_node;
static const size_t size_of_node = sizeof(node);
extern double weights[];
extern int total_spam;
extern int total_ham;
static struct AdditionalInformation ai;

/* *
 * Initialization and prepration
 * Should be called before function train/predict, or it won't work.
 *
 * @param db_path -> path of the database file, if the path dose not exist, then a new database is created.
 * @param size_of_cache -> the size of the memory map region, the unit is byte, you can tune the whole
 *		memory consumption by tune this parameter, the base memory consumption is around 3.5MB, the larger
 *		this parameter is, the larger whole memory consumption is, while it runs faster.
 * @return true if succeed, or false
 * */
bool terminator_init(const string db_path, size_t size_of_cache);

/* *
 * Initialization of database
 * @return true if succeed, or false
 * */
bool init_db(const string db_path, size_t size_of_cache);

/* *
 * feature extraction, and vectorization. the model is fecthed from database
 * @param string -> content of e-mail
 * @param map<string, node>& -> hash map which holds the model
 * */
void vectorization(const string, map<string, node>&);

/* *
 * called by train, after trainning, we save the model in tmp_map to db
 * @param map<string, node>& -> hash map which holds the model
 * */
void save_db(map<string, node>&);

/* *
 * should be called after all work done, it then save the addition information in db
 * */
void terminator_exit();

/* *
 * core of terminator, to predict an e-mail
 * @param string -> the content of e-mail
 *	@note the content is the original string of e-mail, have not been decoded.
 *
 * @return a score represent the spamness
 * */
double terminator_predict(string);

/* *
 * training function
 * @param string -> the content of e-mail
 * @param bool -> is spam or not
 *
 * */
void terminator_train(string, bool);

inline double logist(double x)
{
  return 1.0 / (1.0 + exp(-x));
}

inline double invlogist(double x)
{
  return log(x / (1 - x));
}

extern const double algorithm_threshold;
extern const double owv_step;

extern double logist_predict(map<string, node>&);
extern void logist_train(map<string, node>&, bool);

extern double bwinnow_predict(map<string, node>&);
extern void bwinnow_train(map<string, node>&, bool);

extern double nsnb_predict(map<string, node>&);
extern void nsnb_train_cell(map<string, node>&, bool);
extern void nsnb_train(map<string, node>&, bool);

extern double pam_predict(map<string, node>&);
extern void pam_train(map<string, node>&, bool);

extern double pa_predict(map<string, node>&);
extern void pa_train(map<string, node>&, bool);

extern double winnow_predict(map<string, node>&);
extern void winnow_train(map<string, node>&, bool);

extern double hit_predict(map<string, node>&);
extern void hit_train(map<string, node>&, bool);

extern double nb_predict(map<string, node>&);
extern void nb_train(map<string, node>&, bool);
extern void nb_train_cell(map<string, node>&, bool);

/* *
 * the set of classifiers
 * */
static struct combined_setting
{
  double
  (*predictors[NUM_OF_CLASSIFIERS])(map<string, node>&);
  void
  (*trainers[NUM_OF_CLASSIFIERS])(map<string, node>&, bool);
  unsigned num_of_classifiers;
} filters =
{
  {
    nb_predict, hit_predict, winnow_predict, bwinnow_predict, nsnb_predict,
    pa_predict, pam_predict, logist_predict
  },
  {
    nb_train, hit_train, winnow_train, bwinnow_train, nsnb_train, pa_train,
    pam_train, logist_train
  }, NUM_OF_CLASSIFIERS
};


#endif /* COMMEN_H_ */
