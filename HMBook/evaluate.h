//                                                                    -*-c++-*-
// $Id: evaluate.h 2216 2008-06-13 21:13:47Z distler $
//
#define TRUE 1
#define FALSE 0

class HMexpression { 
protected:
  void *result;                            // contains result of evaluation
  bool valid;                              // TRUE if already evaluated
public: 
  HMexpression() { valid = FALSE;}; 
  virtual ~HMexpression() {}; //{if (valid) delete result;};
  void *res() { 
    if (!valid) {
      result = eval();
      valid = TRUE; 
    } return result;
  }
  double      evaluate()   { return * (double      *) res();};
  virtual void reset() = 0;
  virtual void *eval() = 0;
};

template <class data> class HMliteral : public HMexpression {
public:
  data value;
  HMliteral(data f) { value = f; };
  virtual void reset() {if (valid) delete (data *) result; valid = FALSE;};
  void *eval();
  data *operator&(){return &value;};
};

template <class data> class HMbin : public HMexpression {
private:
  data (*operation)(data left, data right);
public:
  HMexpression *left, *right;
  void reset() {
    if (valid) delete (data *) result; 
    valid = FALSE;
    left->reset();
    right->reset();
  }
  HMbin(HMexpression *a, HMexpression *b, data (*op)(data, data)) {
    left=a;
    right=b;
    operation=(data (*)(data, data)) op;}
  void *eval();
};

template <class Result, class arg> class HMfunc : public HMexpression {
private:
  Result (*operation)(arg a);
  HMexpression *argument;
public:
  void reset() {
    if (valid) delete (Result *) result; 
    valid = FALSE;
    argument->reset();
  };
  HMfunc(HMexpression *ag, Result (*op)(arg)) {
    argument=ag; operation=op; 
  }
  void *eval();
};

#define returnv return (void *) new 














