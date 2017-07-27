/*
 * This file is part of the Gerberoid project.
 *
 * Copyright (C) 2017 Marcus Comstedt <marcus@mc.pp.se>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "wx/string.h"

struct IO_ERROR
{
    IO_ERROR() {}
};

struct OUTPUTFORMATTER
{
};

#define LINE_READER_LINE_INITIAL_SIZE       5000
#define LINE_READER_LINE_DEFAULT_MAX        100000

class FILE_LINE_READER
{
 private:
  FILE *fp;
  unsigned capacity;
  char* line;

 public:
  FILE_LINE_READER( FILE* aFile, const wxString& aFileName)
    : fp(aFile), capacity(LINE_READER_LINE_INITIAL_SIZE)
  {
    if (ftell(aFile) == 0)
      setvbuf(fp, NULL, _IOFBF, BUFSIZ*8);
    line = new char[capacity+1];
    line[0] = 0;
  }

  ~FILE_LINE_READER()
  {
    if (fp)
      fclose(fp);
    delete[] line;
  }

  char* Line() const { return line; }

  char* ReadLine()
  {
    unsigned length = 0;
    int c;
    do {
      if (length >= LINE_READER_LINE_DEFAULT_MAX) {
	line[0] = 0;
	return NULL;
      }
      if (length >= capacity) {
	capacity *= 2;
	if (capacity > LINE_READER_LINE_DEFAULT_MAX)
	  capacity = LINE_READER_LINE_DEFAULT_MAX;
	char *newline = new char[capacity+1];
	memcpy(newline, line, length);
	delete[] line;
	line = newline;
      }
      c = getc_unlocked(fp);
      if (c == EOF)
	break;
      line[length++] = c;
    } while(c != '\n');
    line[length] = 0;
    return length? line : NULL;
  }
};

inline int StrPrintf( std::string* aResult, const char* aFormat, ...)
  __attribute__((__format__(__printf__, 2, 3)));
inline int StrPrintf( std::string* aResult, const char* aFormat, ...)
{
  va_list va;
  va_start(va, aFormat);
  wxString tmp;
  int result = tmp.PrintfV(aFormat, va);
  if (aResult) *aResult = tmp;
  va_end(va);
  return result;
}
