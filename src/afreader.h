#ifndef AFREADER_H
#define AFREADER_H

#include <cstdio>

namespace StatModel {
//! File reader with support to omit # starting lines
/**

  @author   Mikkel B. Stegmann, Chen Xing
  @version  2010-01-26
*/
class AFReader
{
public:
    AFReader( const char *filename );
    ~AFReader();

    bool MoreNonWhiteSpaceOnLine();

    /// Skips whitespace and any commments preceeding the current file position.
    void Sync() { SkipWhiteSpace(); SkipComments(); }

    /// Returns true if the file is valid.
    bool IsValid() { return fh!=NULL; }

    /// Current open file.
    FILE *FH() { return fh; }
private:
    FILE *fh;
    void SkipRestOfLine();
    void SkipComments();
    void SkipWhiteSpace();
    const char CR;
    const char LF;
    const char COMMENT_CHAR;
};
}
#endif // AFREADER_H
