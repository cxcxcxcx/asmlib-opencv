#include "afreader.h"

namespace StatModel {
//! Constructor. Opens the file.
/**

  @author   Mikkel B. Stegmann
  @version  4-17-2000


  @param    filename	The file to open.

  @return   Nothing.

*/
AFReader::AFReader( const char *filename ):
        CR(0x0a), LF(0x0d), COMMENT_CHAR('#') {
    fh = fopen( filename, "rb" );
}

//! Destructor. Closes the file.
/**

  @author   Mikkel B. Stegmann
  @version  4-17-2000
  @return   Nothing.

*/
AFReader::~AFReader() {
//    puts("Closing file");
    if (fh) fclose(fh);
}

//! Increments the file pointer beyond any white space.
/**

  @author   Mikkel B. Stegmann
  @version  4-17-2000
  @return   Nothing.

*/
void AFReader::SkipWhiteSpace() {
    int ch;
    do {
        ch=fgetc( fh );
    } while( ch==' ' || ch=='\t' || ch==CR || ch==LF );
    ungetc( ch, fh );
}

//! Increments the file pointer to the start of the next line.
/**
  @author   Mikkel B. Stegmann
  @version  4-17-2000
  @return   Nothing.

*/
void AFReader::SkipRestOfLine() {
    int ch;
    do {
        ch=fgetc( fh );
    } while( ch!=EOF && ch!=CR && ch!=LF );
    ungetc( ch, fh );

    SkipWhiteSpace();
}

//! Returns true if more white space is present on the current line.
/**

  @author   Mikkel B. Stegmann
  @version  4-17-2000

  @return   Nothing.

*/
bool AFReader::MoreNonWhiteSpaceOnLine() {
    char buf[256];
    int ch, n_gets = 0;
    bool non_white = false;

    do {
      ch=fgetc( fh );
      buf[n_gets++] = ch;
      if ( ch!='\t' && ch!=' ' && ch!=CR && ch!=LF && ch!=EOF) {
          non_white = true; break;
      }
    } while( ch!=EOF && ch!=CR && ch!=LF );

    for(int i=0;i<n_gets;i++)
        ungetc( buf[--n_gets], fh );

    return non_white;
}

//! Increments the file pointer beyond any comments.
/**

  @author   Mikkel B. Stegmann
  @version  4-17-2000

  @return   Nothing.

*/
void AFReader::SkipComments() {
    int ch;
    ch = getc( fh );
    if ( ch==COMMENT_CHAR ) {
        SkipRestOfLine();
        SkipComments();
    } else {
        ungetc( ch, fh );
    }
}

} // Namespace