#include "ed.h"
#include <time.h>

Random::Random (long seed)
{
  srandom (seed);
}

void
Random::srandom (long seed)
{
  init_genrand (seed);
}

long
Random::random ()
{
  return genrand_int31 ();
}

lisp
make_random_state (lisp keys)
{
  lisp v = find_keyword (Kdata, keys);
  if (!general_vector_p (v) || xvector_length (v) != Random::INDEX_MAX + 1)
    FEprogram_error (Einvalid_random_state_initializer, v);
  lisp *x = xvector_contents (v);
  for (int i = 0; i < Random::INDEX_MAX + 1; i++, x++)
    if (!fixnump (*x))
      FEprogram_error (Einvalid_random_state_initializer, v);
  x = xvector_contents (v);
  int n = fixnum_value (*x++);
  if (n < 0 || n >= Random::INDEX_MAX)
    FEprogram_error (Einvalid_random_state_initializer, v);
  lrandom_state *p = make_random_state ();
  p->object.index = n;
  for (i = 0; i < Random::INDEX_MAX; i++, x++)
    p->object.X[i] = fixnum_value (*x);
  return p;
}

static lisp
coerce_to_random_state (lisp state)
{
  if (!state || state == Qnil)
    {
      state = xsymbol_value (Vrandom_state);
      if (!random_state_p (state))
        {
          xsymbol_value (Vrandom_state) = xsymbol_value (Vdefault_random_state);
          FEprogram_error (Erandom_state_is_not_random_state, state);
        }
    }
  else
    check_random_state (state);
  return state;
}

/*GENERIC_FUNCTION:NUMBER*/
lisp
Frandom (lisp number, lisp state)
{
  state = coerce_to_random_state (state);
  double d = double (xrandom_state_object (state).random ()) / Random::RANDOM_MAX;
  d = d * coerce_to_double_float (number);
  switch (number_typeof (number))
    {
    case Tshort_intP:
    case Tlong_int:
      return make_fixnum (int (d));

    case Tbignum:
      return make_integer (double_to_bignum_rep (d));

    case Tfraction:
      return flonum_to_rational (d);

    case Tsingle_float:
      return make_single_float (float (d));

    default: // double-float
      return make_double_float (d);
    }
}

lisp
Fmake_random_state (lisp state)
{
  if (state != Qt)
    state = coerce_to_random_state (state);
  lisp p = make_random_state ();
  if (state == Qt)
    xrandom_state_object (p).srandom (time (0));
  else
    xrandom_state_object (p) = xrandom_state_object (state);
  return p;
}

lisp
Frandom_state_p (lisp object)
{
  return boole (random_state_p (object));
}
