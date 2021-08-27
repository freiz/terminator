Introduction
=== 

[Terminator](https://github.com/freiz/terminator) is a library written in C++ for spam filtering, like the famous [SpamBayes](http://spambayes.sourceforge.net/)
and [OSBF-Lua](https://github.com/arunpersaud/osbf-lua). It's suitable to be embeded into other spam filtering software or service as a machine learning module. The advantages are 

* Very high precison and recall, best results on all public spam filtering corpus.
* Controllable memroy usage, suitable for both server side and client side filtering on typical PC.

Terminator can be used in any other binary text classification problems, especially those need adaptive model to do online learning. 

Terminator is **not** a complete set of spam filtering library while only focus on machine learning part without black/white list or dkim and so on. The implementation are described in detail in my paper "[An Adaptive Fusion Algorithm for Spam Detection](http://csse.szu.edu.cn/staff/panwk/publications/Journal-IEEE-IS-14-AFSD.pdf)".

Implementation
===
Most machine learning modules used in exsiting spam filtering softwares use variant navie bayes algorithms. Terminator used a combined model which include eight different machine learning algorithms to boost the spam filtering perfomance. The algorithms are listed below with according papers

* [Naive Bayes](http://classes.soe.ucsc.edu/cmps242/Fall09/lect/12/CEAS2006_corrected-naiveBayesSpam.pdf)
* [Not So Naive Bayes](http://aaai.org/ocs/index.php/IAAI/IAAI09/paper/view/240/1033)
* [Online Logistic Regression](https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/GoodmanYih-ceas06.pdf)
* [HIT](http://www.ceas.cc/2008/papers/china.pdf)
* [Winnow](http://www.cs.cmu.edu/~vitor/papers/kdd06_final.pdf)
* [Balanced Winnow](http://www.cs.cmu.edu/~vitor/papers/kdd06_final.pdf)
* [Passive Aggressive](https://www.cs.huji.ac.il/~shais/papers/CrammerDeShSi03.pdf)
* [On-line Perceptron Algorithm with Margins](http://www.eecs.tufts.edu/~dsculley/papers/trec.2006.spam.pdf)

Installation & Usage
===

### Step 1, Install Dependencies
The only dependencies is [kyotocabinet](http://fallabs.com/kyotocabinet/) for persistance, which need to be installed first.

### Step 2, Install Terminator and Compile
```bash
clone https://github.com/freiz/terminator.git
cd terminator
make
```
You can change the compiler suite in Makefile, the output is a static linkable lib.

### Step 3, Write an Example
```c++
#include "terminator.h"

// The first parameter is the path of database file
// The second parameter is the main memory used as cache, the unit is Byte, so 5 << 20 is around 5MB as cache
Terminator* classifier = new Terminator("terminator.kch", 5 << 20);

// Now you can write the main logic
// There are two public api, Train and Predict

// [Predict] pass in the email content and return a score ranging from 0 (100% ham) to 1 (100% spam)
// You can change the threshold to make the decision on your own 
double score = classifier->Predict(std::string email_content);

// [Train] pass in the email content and a flag
// If spam train, the flag set to true or false
classifier->Train(std::string email_content, boolean is_spam)
```

### Step 4, Play with Demo (Optional)
```bash
make run-demo
```
It will run a demo application to simulate spam filtering using spamassassin corpus, you can also put other corpus (such as [ceas08](http://plg.uwaterloo.ca/~gvcormac/ceascorpus/)) under demo/corpus to check the experiment result.

### Step 5, Compile and Link Your Own bits
Do not forget to link against kyotocabinet.

Experiment Result
===
Here, I only quote samples of results on public corpus [Trec05-p1](http://trec.nist.gov/pubs/trec14/papers/SPAM.OVERVIEW.pdf)

<table>
  <tr>
    <th>Competitor</th><th>(1-ROCA)%, the smaller the better</th>
  </tr>
  <tr>
    <td>bogoﬁlter</td><td>0.048</td>
  </tr>
  <tr>
    <td>spamprobe</td><td>0.059</td>
  </tr>
  <tr>
    <td>spamasassin</td><td>0.059</td>
  </tr>
  <tr>
    <td>terminator</td><td>0.0055</td>
  </tr>
</table>

The paper "[An Adaptive Fusion Algorithm for Spam Detection](http://csse.szu.edu.cn/staff/panwk/publications/Journal-IEEE-IS-14-AFSD.pdf)" contains a very complete set of experiments based on terminator.

