//Copyright (C) 2006 LABORIUS
//Dominic Letourneau, ing., M.Sc.A.
#ifndef _HEXUTILS_H_
#define _HEXUTILS_H_

#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <map>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <algorithm>


/*
FORMAT ":BBAAAATTHHHH...HHHHCC"

Each data record begins with a 9 character prefix and
always ends with a 2 character checksum. All records
begin with a ':'
Regardless of the format. The individual elements are described below.

[BB] - is a two digit hexadecimal byte count representing
the number of data bytes that will appear
on the line. Divide this number by two to get the
number of words per line.

[AAAA] - is a four digit hexadecimal address representing
the starting address of the data record.
Format is high byte first followed by low byte. The
address is doubled because this format only supports
8-bits (to find the real PICmicro address,
simply divide the value AAAA by 2).

This means :  FLASH 



[TT] - is a two digit record type that will be '00' for
data records, '01' for end of file records and '04'
for extended address record (INHX32 only).

Extended records will produce 2 data bytes
containing the msb bytes for the 32 bits address

[HHHH] - is a four digit hexadecimal data word. Format
is low byte followed by high byte. There will
be BB /2 data words following TT.

[CC] - is a two digit hexadecimal checksum that is
the two's complement of the sum of all the preceding
bytes in the line record.

*/

namespace hexutils 
{

    typedef enum _hexrecordtype_ {DATA_RECORD=0x00, 
                         END_OF_FILE=0x01, 
                         EXTENDED_ADDRESS=0x04} HexRecordType;

    std::map<unsigned char, unsigned char> buildhexmap();
    
    unsigned char getHexValue(const unsigned char hexdigit);
    
    std::ostream &sethex(std::ostream &stream);
    
    // Reset flags.
    std::ostream &resethex(std::ostream &stream);
    
    
    class hex32line
    {
     public:
                          
         //Constructor with string containing hex32 data
         hex32line(const char* buffer, const int max_size);
            
         bool operator< (const hex32line &line) const;
            
         //Copy constructor          
         hex32line(const hex32line &cpy);
         
         unsigned char operator [] (int pos);
         
         void print(std::ostream &out);
         
         size_t size() const;
          
         unsigned char getNbBytes() const;
         
         std::vector<unsigned char> & getData();
         
         unsigned int getAddress() const;
         
         void setAddress(unsigned int address);
            
         unsigned char getType() const;
         
         unsigned char getChecksum();
         
         bool verifyChecksum();
            
     private:
    
         //default constructor not allowed
         hex32line();
         
         unsigned short convertHexToUShort(const std::string &hexstring, bool swap = 0);

         unsigned char convertHexToUChar(const std::string &hexstring);

          //hex32 data will appear in this sequence   
          unsigned short m_nbBytes;
          unsigned int m_address;
          unsigned short m_type;
          std::vector<unsigned char> m_data;      
          unsigned char m_checksum;
    };
    
    class hex32doc
    {
     public:
         
         hex32doc();
         
         hex32doc(const std::string &filename);
         
         void print(std::ostream &out);
         
         void clear();
         
         void load(const std::string &filename);
                  
         void parse();
         
         size_t size();

         size_t dataSize() const;
         
         hex32line& getLine(unsigned int no);

         bool validate();
                
     private:
                  
         void sort_lines();
                          
         std::vector<hex32line> m_lines;    
         
         unsigned int last_msb;    
         
    };

}//namespace hexutils

#endif
