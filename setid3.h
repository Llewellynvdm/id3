/*

  set_tag::ID3 applicative class

  (c) 2004, 2005 squell ^ zero functionality!
  see the file 'COPYING' for license conditions

  Usage:

  The set_tag::ID3 class implements the single_tag interface for ID3 tags

  Example:

  int main(int argc, char* argv[])
  {
      set_tag::ID3()
      .set(artist, "%2")
      .set(title,  "%3")
      .modify(argv[1], argv);
  }

*/

#ifndef __ZF_SETID3
#define __ZF_SETID3

#include <string>
#include <vector>
#include "set_base.h"

namespace set_tag {

class ID3 : public single_tag, public provider {
    std::vector<const char*> mod;    // modification data
    bool fresh;                      // should vmodify clear existing tag?
public:
    ID3() : mod(FIELDS,(char*)0), fresh(false)
    { }

    bool    vmodify(const char*, const subst&) const;
    reader* read(const char*) const;

  // standard set

    ID3& set(ID3field i, const char* m)     // set ID3 field i to value m
    { if(i < FIELDS && !mod[i]) mod[i] = m; return *this; }

    ID3& clear()
    { fresh = true; return *this; }
};

}

#endif

