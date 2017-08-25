#include <Rcpp.h>
using namespace Rcpp;

#include <sstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <codecvt>

#include "pole.h"

std::string MSG_UTF16LE = std::string("001F");
std::string MSG_BINARY = std::string("0102");
std::string MSG_PROPERTIES = std::string("properties_version1.0");

template <typename T>
std::string toUTF8(const std::basic_string<T, std::char_traits<T>, std::allocator<T> >& source) {

  std::string result;

  std::wstring_convert<std::codecvt_utf8_utf16<T>, T> convertor;
  result = convertor.to_bytes(source);

  return result;

}

bool ends_with(const std::string& a, const std::string& b) {
  if (b.size() > a.size()) return false;
  return std::equal(a.begin() + a.size() - b.size(), a.end(), b.begin());
}

List visit(int indent, POLE::Storage* storage, std::string path) {

  std::vector<std::string> keys;
  List vals;

  std::list<std::string> entries;
  entries = storage->entries(path);

  std::list<std::string>::iterator it;

  for (it = entries.begin(); it != entries.end(); ++it) {

    std::string name = *it;
    std::string fullname = path + name;

    // for (int j = 0; j < indent; j++) std::cout << "    " ;

    POLE::Stream* ss = new POLE::Stream(storage, fullname);

    // std::cout << fullname;

    if (!(storage->isDirectory(fullname))) {

      unsigned char *buf = (unsigned char *)malloc(ss->size());

      unsigned read = ss->read(buf, ss->size());

      if (read > 0) {

        if (ends_with(fullname, MSG_UTF16LE)) { // UTF-16LE string

          int wlen = (ss->size())/2;
          char16_t *dest = new char16_t[wlen + 1];
          unsigned char *ptr = buf;

          for (int i=0; i<wlen; i++) {
            dest[i] = (ptr[1] << 8) + ptr[0];
            ptr += 2;
          }

          dest[wlen] = 0;

          keys.push_back(fullname);
          vals.push_back(toUTF8(std::u16string(dest)));

          // std::cout << "===============>" << toUTF8(std::u16string(dest));

          delete[] dest;

        } else if (ends_with(fullname, MSG_BINARY)) { // binary content

          keys.push_back(fullname);
          vals.push_back(RawVector(buf, buf+read));

        } else if (ends_with(fullname, MSG_PROPERTIES)) { // properties content

          keys.push_back(fullname);
          vals.push_back(RawVector(buf, buf+read));

        }

      }

      // std::cout << std::endl;

      if (buf) free(buf);

    }

    delete ss;

    if (storage->isDirectory(fullname)) {

      List res = visit(indent+1, storage, fullname + "/");

      std::vector<std::string> rk = res["keys"];

      if (rk.size() > 0) {

        keys.insert(keys.end(), rk.begin(), rk.end());

        List::iterator lit;
        List rv = res["values"];

        for (lit = rv.begin(); lit != rv.end(); ++lit) {
          vals.push_back(*lit);
        }

      }

    }

  }

  return(
    List::create(
      Named("keys") = keys, Named("values") = vals
    )
  );

}


// [[Rcpp::export]]
List int_read_msg(std::string path) {

  POLE::Storage* storage = new POLE::Storage(path.c_str());

  storage->open();

  if (storage->result() == POLE::Storage::Ok) {
    return(visit(0, storage, "/"));
  } else {
    return(wrap(NULL));
  }

}
