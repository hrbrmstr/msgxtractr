#include <Rcpp.h>

using namespace Rcpp;

#include <sstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <cstring>

#include "pole.h"
#include "utf8.h"

#include "alloc.h"
#include "util.h"
#include "tnef.h"
#include "rtf.h"

std::string MSG_UTF16LE = std::string("001F");
std::string MSG_BINARY = std::string("0102");
std::string MSG_TIME = std::string("0040");
std::string MSG_PROPERTIES = std::string("properties_version1.0");

unsigned int get_int_32(unsigned char *p){
  return (unsigned int)((unsigned char)(p)[0] + ((unsigned char)(p)[1]<<8) + ((unsigned char)(p)[2]<<16) + ((unsigned char)(p)[3]<<24));
}

//' @export
// [[Rcpp::export]]
bool is_rtf(RawVector v) {

  static const unsigned int rtf_uncompressed_magic = 0x414c454d;
  static const unsigned int rtf_compressed_magic =   0x75465a4c;

  size_t compr_size = 0L;
  size_t uncompr_size = 0L;
  unsigned int magic;
  size_t idx = 0;

  unsigned char *data = (unsigned char *)(v.begin());

  compr_size = get_int_32(data + idx); idx += 4;
  uncompr_size = get_int_32(data + idx); idx += 4;
  magic = get_int_32(data + idx); idx += 4;

  return((magic == rtf_uncompressed_magic) || (magic == rtf_compressed_magic));

}

static const char* rtf_prebuf = "{\\rtf1\\ansi\\mac\\deff0\\deftab720{\\fonttbl;}{\\f0\\fnil \\froman \\fswiss \\fmodern \\fscript \\fdecor MS Sans SerifSymbolArialTimes New RomanCourier{\\colortbl\\red0\\green0\\blue0\r\n\\par \\pard\\plain\\f0\\fs20\\b\\i\\u\\tab\\tx";

RawVector decompress_rtf_data(unsigned char *src, size_t lenc, size_t lenu) {

  const size_t rtf_prebuf_len = strlen( rtf_prebuf );

  int woff, eoff, roff, rlen;
  int control, cin, cout, i, j, endflag;
  RawVector v(lenu);
  unsigned char *dest = &v[0];
  unsigned char  dict[4096];

  /* setup dictionary */

  memset( dict, 0x0, sizeof(dict) );
  memmove( dict, rtf_prebuf, rtf_prebuf_len );

  woff = rtf_prebuf_len;
  eoff = rtf_prebuf_len;

  /* setup destination */

  assert( lenu>0 );		/* sanity check */

//  dest = CHECKED_XCALLOC(unsigned char, lenu);
  cout = 0;

  /* setup source */

  assert( lenc>0 );

  cin = 0;

  /* processing loop */

  endflag = 0;

  while (1) {

    if ( endflag ) break;

    /* get control byte */ //Rcout<<"control byte"<<std::endl;
    if ( cin+1 > lenc ) {
      endflag = -1;
      break;			/* input overrun */
    }

    control = (int)src[cin++];

    /* handle control run */
    for ( i=0; i<8; i++ ) {

      if ( endflag ) break;

      if ( control & (1<<i) ) {

        /* dictionary reference */ //Rcout<<"dict ref"<<std::endl;
        if ( cin+2 > lenc ){
          endflag = -1;
          break;		/* input overrun */
        }

        roff = (int)src[cin++];
        rlen = (int)src[cin++];

        roff = (roff<<4) + (rlen>>4);
        rlen = (rlen&0x0f) + 2;

        /* the one true exit test */ //Rcout<<"exit test"<<std::endl;
        if ( roff == woff ) {
          endflag = 1;
          break;		/* happy ending */
        }

        /* handle reference */ //Rcout<<"ref"<<std::endl;
        if ( cout+rlen > lenu ) {
          endflag = -1;
          break;		/* output overrun */
        }

        for ( j=0; j<rlen; j++ ) {
          dest[cout++] = dict[roff];
          dict[woff++] = dict[roff++];

          roff &= 0xfff;
          woff &= 0xfff;
          if ( eoff < 4096 ) eoff++;
        }

      } else {

        /* literal */ //Rcout<<"literal try"<<std::endl;
        if ( cin+1 > lenc ) {
          endflag = -1;
          break;		/* input overrun */
        }

        if ( cout+1 > lenu ) {
          endflag = -1;
          break;		/* output overrun */
        }

        /* handle literal */ //Rcout<<"literal"<<std::endl;
        dest[cout++] = src[cin];
        dict[woff++] = src[cin++];

        woff &= 0xfff;
        if ( eoff < 4096 ) eoff++;

      }

    }

  }

  if ( endflag < 0 ) Rcpp::Rcout << "RTF buffer overrun, input file may be corrupted" << std::endl;

  return(v);

}

//' @export
// [[Rcpp::export]]
RawVector decode_rtf(RawVector v) {

  static const unsigned int rtf_uncompressed_magic = 0x414c454d;
  static const unsigned int rtf_compressed_magic =   0x75465a4c;

  size_t compr_size = 0L;
  size_t uncompr_size = 0L;
  unsigned int magic;
  size_t idx = 0;

  unsigned char *data = (unsigned char *)&v[0];

  compr_size = get_int_32(data + idx); idx += 4;
  uncompr_size = get_int_32(data + idx); idx += 4;
  magic = get_int_32(data + idx); idx += 4;

  if (magic == rtf_compressed_magic) {

    Rcout << "COMPRESSED! " << compr_size << " " << uncompr_size << std::endl;

    return(decompress_rtf_data(data, compr_size, uncompr_size));

  }

  return(v);

}

bool ends_with(const std::string& a, const std::string& b) {
  if (b.size() > a.size()) return false ;
  return std::equal(a.begin() + a.size() - b.size(), a.end(), b.begin()) ;
}

List visit(POLE::Storage* storage, std::string path) {

  std::vector<std::string> keys;
  List vals;

  std::list<std::string> entries;
  entries = storage->entries(path);

  std::list<std::string>::iterator it;

  for (it = entries.begin(); it != entries.end(); ++it) {

    std::string name = *it;
    std::string fullname = path + name;

    // std::cout << "  Opening Stream " ;

    POLE::Stream* ss = new POLE::Stream(storage, fullname);

    if (!(storage->isDirectory(fullname))) {

      unsigned char *buf = (unsigned char *)malloc(ss->size());

      unsigned read = ss->read(buf, ss->size());

      if (read > 0) {

        // Rcpp::Rcout << "NAME: " << fullname << std::endl;

        if (ends_with(fullname, MSG_UTF16LE)) { // UTF-16LE string

          // all this to make the string content useful

          int wlen = (ss->size())/2;
          char16_t *dest = new char16_t[wlen + 1];
          unsigned char *ptr = buf;

          for (int i=0; i<wlen; i++) {
            dest[i] = (ptr[1] << 8) + ptr[0];
            ptr += 2;
          }

          dest[wlen] = 0;

          keys.push_back(fullname);

          std::vector<unsigned char> utf8result;
          utf8::utf16to8(dest, dest + wlen, std::back_inserter(utf8result));
          vals.push_back(std::string(utf8result.begin(), utf8result.end()));

          delete[] dest;

        } else if (ends_with(fullname, MSG_BINARY)) { // binary content

          keys.push_back(fullname);
          vals.push_back(RawVector(buf, buf+read));

        } else if (ends_with(fullname, MSG_TIME)) { // time content

          keys.push_back(fullname);
          vals.push_back(RawVector(buf, buf+read));

        } else if (ends_with(fullname, MSG_PROPERTIES)) { // properties content

          keys.push_back(fullname);
          vals.push_back(RawVector(buf, buf+read));

        } else {

          std::cout << fullname.c_str() << std::endl;

        }

      }

      if (buf) free(buf);

    }

    // std::cout << "- Deleting Stream" << std::endl;
    delete ss;

    if (storage->isDirectory(fullname)) {

      List res = visit(storage, fullname + "/");

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

  // std::cout << "Opening File" << std::endl ;

  POLE::Storage* storage = new POLE::Storage(path.c_str());

  storage->open();

  if (storage->result() == POLE::Storage::Ok) {

    List l = visit(storage, "/");

    if (storage) {
      // std::cout << "Closing File" << std::endl << std::endl;
      storage->close();
      delete storage;
    }

    return(l);

  } else {

    if (storage) {
      // std::cout << "Closing File" << std::endl << std::endl;
      storage->close();
      delete storage;
    }

    return(wrap(NULL));

  }

}
