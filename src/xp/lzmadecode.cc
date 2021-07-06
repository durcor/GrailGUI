#include <errno.h>
#include <lzma.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
using namespace std;

// Initialize a .xz decoder. The decoder supports a memory usage limit
// and a set of flags.
//
// The memory usage of the decompressor depends on the settings used
// to compress a .xz file. It can vary from less than a megabyte to
// a few gigabytes, but in practice (at least for now) it rarely
// exceeds 65 MiB because that's how much memory is required to
// decompress files created with "xz -9". Settings requiring more
// memory take extra effort to use and don't (at least for now)
// provide significantly better compression in most cases.
//
// Memory usage limit is useful if it is important that the
// decompressor won't consume gigabytes of memory. The need
// for limiting depends on the application. In this example,
// no memory usage limiting is used. This is done by setting
// the limit to UINT64_MAX.
//
// The .xz format allows concatenating compressed files as is:
//
//     echo foo | xz > foobar.xz
//     echo bar | xz >> foobar.xz
//
// When decompressing normal standalone .xz files, LZMA_CONCATENATED
// should always be used to support decompression of concatenated
// .xz files. If LZMA_CONCATENATED isn't used, the decoder will stop
// after the first .xz stream. This can be useful when .xz data has
// been embedded inside another file format.
//
// Flags other than LZMA_CONCATENATED are supported too, and can
// be combined with bitwise-or. See lzma/container.h
// (src/liblzma/api/lzma/container.h in the source package or e.g.
// /usr/include/lzma/container.h depending on the install prefix)
// for details.
static bool init_decoder(lzma_stream *strm) {
  lzma_ret ret = lzma_stream_decoder(strm, UINT64_MAX, LZMA_CONCATENATED);

  // Return successfully if the initialization went fine.
  if (ret == LZMA_OK) return true;
  throw ret;  // break out with the appropriate lZMA error
}

// When LZMA_CONCATENATED flag was used when initializing the decoder,
// we need to tell lzma_code() when there will be no more input.
// This is done by setting action to LZMA_FINISH instead of LZMA_RUN
// in the same way as it is done when encoding.
//
// When LZMA_CONCATENATED isn't used, there is no need to use
// LZMA_FINISH to tell when all the input has been read, but it
// is still OK to use it if you want. When LZMA_CONCATENATED isn't
// used, the decoder will stop after the first .xz stream. In that
// case some unused data may be left in strm->next_in.
static void decompress(lzma_stream *strm, const char *inname, FILE *infile,
                       FILE *outfile) {
  lzma_action action = LZMA_RUN;

  uint8_t inbuf[BUFSIZ];
  uint8_t outbuf[BUFSIZ];

  strm->next_in = NULL;
  strm->avail_in = 0;
  strm->next_out = outbuf;
  strm->avail_out = sizeof(outbuf);

  while (true) {
    if (strm->avail_in == 0 && !feof(infile)) {
      strm->next_in = inbuf;
      strm->avail_in = fread(inbuf, 1, sizeof(inbuf), infile);

      if (ferror(infile)) {
        throw "Read error";
      }

      // Once the end of the input file has been reached,
      // we need to tell lzma_code() that no more input
      // will be coming. As said before, this isn't required
      // if the LZMA_CONATENATED flag isn't used when
      // initializing the decoder.
      if (feof(infile)) action = LZMA_FINISH;
    }

    lzma_ret ret = lzma_code(strm, action);

    if (strm->avail_out == 0 || ret == LZMA_STREAM_END) {
      size_t write_size = sizeof(outbuf) - strm->avail_out;

      if (fwrite(outbuf, 1, write_size, outfile) != write_size) {
        throw "Write error";  // strerror(errno));
      }

      strm->next_out = outbuf;
      strm->avail_out = sizeof(outbuf);
    }

    if (ret != LZMA_OK) {
      // Once everything has been decoded successfully, the
      // return value of lzma_code() will be LZMA_STREAM_END.
      // It is important to check for LZMA_STREAM_END. Do not
      // assume that getting ret != LZMA_OK would mean that
      // everything has gone well or that when you aren't
      // getting more output it must have successfully
      // decoded everything.
      if (ret == LZMA_STREAM_END) return;

      // It's not LZMA_OK nor LZMA_STREAM_END,
      // so it must be an error code. See lzma/base.h
      // (src/liblzma/api/lzma/base.h in the source package
      // or e.g. /usr/include/lzma/base.h depending on the
      // install prefix) for the list and documentation of
      // possible values. Many values listen in lzma_ret
      // enumeration aren't possible in this example, but
      // can be made possible by enabling memory usage limit
      // or adding flags to the decoder initialization.
      throw ret;
    }
  }
}

int main(int argc, char *argv[]) {
  const char *filename = argc > 1 ? argv[1] : "uscounties.bml.lzma";
  try {
    lzma_stream strm = LZMA_STREAM_INIT;
    init_decoder(&strm);

    FILE *infile = fopen(filename, "rb");

    if (infile == nullptr) throw "can't open file";
    decompress(&strm, argv[1], infile, stdout);
    fclose(infile);
    lzma_end(&strm);  // Free the memory after last file decoded
    if (fclose(stdout)) throw "can't close file";
  } catch (const char msg[]) {
    cerr << msg << '\n';
  } catch (lzma_ret r) {
    cerr << "LZMA error: " << r << '\n';
  }
}
