/*

  character conversion (user locale <-> latin1)

  copyright (c) 2005, 2006 squell <squell@alumina.nl>

  use, modification, copying and distribution of this software is permitted
  see the accompanying file 'COPYING' for license conditions

  Usage:

  The cvtstring class encapsulates an encoding-neutral string. Uses templates
  heavily for avoiding compile dependencies and using the C++ type system
  itself for specifying conversions. A limited string interface is provided
  for constructing, but not editing strings.

  Example:

  std::puts( conv<latin1>("ISO 8859-1 text").c_str<local>() );

*/

#ifndef __ZF_CHARCONV_HPP
#define __ZF_CHARCONV_HPP

#include <cstddef>
#include <string>

  /* most older gcc's don't define wstring, but I only need it as a
     simple and adequate container, and it works just fine. */

#if (__DJGPP__) || (__GNUC__ == 2)
namespace std {
    typedef basic_string<wchar_t> wstring;
}
#endif

namespace charset {
    template<class Encoding = void> class conv;

  /*
      Making the default template the base class for non default templates
      solves the problem of dynamically specifying conversions, removes
      the need for a template conversion constructor, and moves error messages
      from the linking stage to the compiler stage.
  */

    template<> class conv<void> {
    protected:
        typedef std::wstring data;
        static const int cellsize = sizeof(wchar_t) / sizeof(data::value_type);

        template<class T> struct proxy {                // value wrapper
            typedef typename conv<T>::char_type char_t;
            operator const char_t*() const { return str.c_str(); }
            proxy(const std::basic_string<char_t>& s) : str(s) { }
        private:
            const std::basic_string<char_t> str;
        };

        data internal;
        explicit conv(const data& s) : internal(s) { }
    public:
        conv(const conv<>& other) : internal(other.internal) { }
        conv(void)                : internal()               { }

        bool empty() const          { return internal.empty(); }
        void clear()                { internal.erase(); }
        void swap(conv<>& other)    { internal.swap(other.internal); }

        void reserve(data::size_type req = 0)
                                    { internal.reserve(req * cellsize); }
        conv<>& operator+=
          (wchar_t c)               { return internal.append((data::value_type*)&c, cellsize), *this; }
        conv<>& operator+=
          (const conv<>& rhs)       { return (internal+=rhs.internal), *this; }

        template<class E>
          std::basic_string<typename conv<E>::char_type>
                   str()   const    { return conv<E>(*this); }
        template<class E>
          proxy<E> c_str() const    { return str<E>(); }
    };

    inline conv<> operator+(conv<> lhs, const conv<>& rhs)
    {
        return lhs += rhs;
    }

  /*
      Any parameterization simply is a different "face" of the same class.
  */

    template<class Encoding> class conv : public conv<> {
    public:
        conv(const std::string& s)         : conv<>(decode(s.data(), s.size())) { }
        conv(const char* p, std::size_t l) : conv<>(decode(p,l)) { }
        conv(const char* p)                : conv<>((conv)std::string(p)) { }
        conv(const conv<>& other)          : conv<>(other) { }
        conv(void)                         : conv<>() { }

        operator std::string const() const
        { return encode(internal.data(), internal.size()/cellsize); }

        std::string const str() const { return *this; }
        proxy<char> c_str()     const { return str(); }

        template<class E>     // some compilers dont like using conv<>::str?
          std::basic_string<typename conv<E>::char_type>
                   str()   const    { return conv<>::str<E>(); }
        template<class E>
          proxy<E> c_str() const    { return conv<>::c_str<E>(); }

        typedef char char_type;
    public:           // too many compilers crap on template friend templates
        static conv<>::data decode(const char*, std::size_t);
        static std::string  encode(const void*, std::size_t);
    };

  /*
      Direct wide char interface
  */

    template<> class conv<wchar_t> : public conv<> {
    public:
        conv(const std::wstring& s)           : conv<>(s) { }
        conv(const wchar_t* p, std::size_t l) : conv<>(std::wstring(p,l)) { }
        conv(const wchar_t* p)                : conv<>(p) { }
        conv(const conv<>& other)             : conv<>(other) { }
        conv(void)                            : conv<>() { }
        conv(std::size_t n, wchar_t c)        : conv<>(std::wstring(n,c)) { }

        operator std::wstring const() const { return internal; }
        std::wstring const str()      const { return internal; }
  //    const wchar_t* c_str()        const { return internal.c_str(); }

        template<class E>
          std::basic_string<typename conv<E>::char_type>
                   str()   const    { return conv<>::str<E>(); }
        template<class E>
          proxy<E> c_str() const    { return conv<>::c_str<E>(); }

        typedef wchar_t char_type;
    };

  /*
      Convenient function
  */

    template<class In, class Out>
    inline std::basic_string<typename Out::char_type> recode(std::basic_string<typename In::char_type> str)
    {
        return (conv<Out>) (conv<In>) str;
    }

  /*
      Predefined charsets, explicit specialization prototypes
  */

    typedef char local;
    struct latin1;

    template<> conv<>::data conv<local >::decode(const char*, std::size_t);
    template<> std::string  conv<local >::encode(const void*, std::size_t);

    template<> conv<>::data conv<latin1>::decode(const char*, std::size_t);
    template<> std::string  conv<latin1>::encode(const void*, std::size_t);
}

#endif

