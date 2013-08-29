#ifndef COMMEN_H_
#define COMMEN_H_

#include <cmath>
#include <map>
#include <string>
#include <kchashdb.h>
#include "terminator_common.h"

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




#endif /* COMMEN_H_ */
