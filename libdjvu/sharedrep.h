/* -*- C -*-
// -------------------------------------------------------------------
// Wrapper for shared representation classes.
// Copyright (c) 2008  Leon Bottou. All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// ------------------------------------------------------------------- */


#ifndef SHAREDREP_H
#define SHAREDREP_H 1

#include "atomic.h"

template <class Rep> 
class SharedRep
{
private:
  Rep *q;
  static inline Rep *ref(Rep *q) 
  { if (q) atomicIncrement(&q->refcount); return q; }
  static inline void *deref(Rep *q)
    { if (q && !atomicDecrement(&q->refcount)) delete q; }
public:
  SharedRep(Rep *rep) 
    : q(rep) { if (q) q->refcount = 1; }
  SharedRep(const SharedRep<Rep> &other) 
    : q(ref(other.q)) {}
  ~SharedRep() 
    { Rep *oq = atomicExchangePointer((void**)&q, 0); deref(q); }
  SharedRep& operator=(const SharedRep<Rep> &other) 
    { Rep *oq = atomicExchangePointer((void**)&q, ref(other.q)); deref(oq); }
  const Rep *rep() const
    { return q; }
  Rep *detach() 
    { if (q->refcount>1) { 
        Rep *nq = q->copy(); nq->refcount=1; 
        Rep *oq = atomicExchangePointer((void**)&q, nq); deref(oq); } 
      return q; }
};



// Recommended usage
//
// #include <cstdlib>
// #include <cstring>
// 
// class String
// {
// private:
//
//   struct Rep
//   {
//     int refcount;
//     int length;
//     char *data;
//     Rep(const char *s, int l)
//       : length(len), data(new char[l+1]) 
//       { ::memcpy(data, s, l);  data[len] = 0; }
//     Rep *copy()
//       { return new Rep(data, length); }
//   };
//
//   SharedRep<Rep> w;
//     
// public:
//   String(const char *s, int l)
//     : w(new Rep(s,l)) {}
//   String(const char *s)
//     : w(new Rep(s,::strlen(s))) {}
// 
//   // function that do not mutate
//   int size() const { return rep()->length; }
//   operator const char*() const { return w.rep()->data; }
//   char operator[](int i) const { return w.rep()->data[i]; }
//   
//   // functions that perform a mutation
//   void set(int i, char c) { w.detach()->data[i] = c; }
// }
// 

#endif



/* -------------------------------------------------------------
   Local Variables:
   c++-font-lock-extra-types: ( "\\sw+_t" "[A-Z]\\sw*[a-z]\\sw*" )
   End:
   ------------------------------------------------------------- */
