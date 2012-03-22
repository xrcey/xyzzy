// -*-C++-*-
#ifndef _random_h_
# define _random_h_

# include "mt19937ar.h"

/* random state */

class Random : public MT
{
public:
  enum {INDEX_MAX = N};
  enum {DEFAULT_SEED = 5489UL};
  enum {RANDOM_BITS = BITS_PER_LONG - 1};
  enum {RANDOM_MAX = LONG_MAX & ~1};

  Random (long = DEFAULT_SEED);
//  Random (Random &);

  void srandom (long);
  long random ();
};

class lrandom_state: public lisp_object
{
public:
  Random object;
  ~lrandom_state () { if (object.X) delete object.X; }
};

# define random_state_p(X) typep ((X), Trandom_state)

inline void
check_random_state (lisp x)
{
  check_type (x, Trandom_state, Qrandom_state);
}

inline Random &
xrandom_state_object (lisp x)
{
  assert (random_state_p (x));
  return ((lrandom_state *)x)->object;
}

inline lrandom_state *
make_random_state ()
{
  lrandom_state* p = ldata <lrandom_state, Trandom_state>::lalloc ();
  p->object.index = Random::INDEX_MAX + 1;
  p->object.X = new unsigned long[Random::INDEX_MAX];
  p->object.mag01[0] = 0x0UL;
  p->object.mag01[1] = MATRIX_A;

  return p;
}

lisp make_random_state (lisp);

#endif
