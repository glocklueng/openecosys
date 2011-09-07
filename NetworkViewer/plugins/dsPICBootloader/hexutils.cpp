#include "hexutils.h"



namespace hexutils 
{


std::map<unsigned char, unsigned char> buildhexmap()
{
   std::map<unsigned char, unsigned char> hexmap;   
   hexmap['0'] = 0;
   hexmap['1'] = 1;
   hexmap['2'] = 2;
   hexmap['3'] = 3;
   hexmap['4'] = 4;
   hexmap['5'] = 5;
   hexmap['6'] = 6;
   hexmap['7'] = 7;
   hexmap['8'] = 8;
   hexmap['9'] = 9;
   hexmap['A'] = 10;
   hexmap['B'] = 11;
   hexmap['C'] = 12;
   hexmap['D'] = 13;
   hexmap['E'] = 14;
   hexmap['F'] = 15;      
   return hexmap;            
}
    
unsigned char getHexValue(const unsigned char hexdigit)
{
       //upper case
       unsigned char hexvalue = toupper(hexdigit);
       
       //Initialize map (only once)
       static std::map<unsigned char, unsigned char> hexmap = buildhexmap();       
       
       if (hexmap.find(hexvalue) != hexmap.end())
       {
          return hexmap[hexvalue];                          
       }
       else
       {
           std::cerr<<"getHexMap invalid hexvalue : "<<hexvalue<<std::endl;    
       }
       return 0xFF;
}
    
std::ostream &sethex(std::ostream &stream)
{
  stream.unsetf(std::ios::dec | std::ios::oct);
  stream.setf(std::ios::hex | std::ios::uppercase | std::ios::showbase);    
  return stream;
}
    
// Reset flags.
std::ostream &resethex(std::ostream &stream)
{
  stream.unsetf(std::ios::hex | std::ios::uppercase | std::ios::showbase);
  stream.setf(std::ios::dec);
  return stream;
}
    

                          
 //Constructor with string containing hex32 data
 hex32line::hex32line(const char* buffer, const int max_size)
 {
      if (max_size < 2)
      {
         std::cerr<<"hex32line::hex32line(const char* buffer, const int max_size) "<<
         "buffer size too small (< 2 bytes)"<<std::endl;
         return;             
      }

      m_nbBytes =  convertHexToUChar(std::string(buffer,2)); 
      
      //buffer complete size verification
      if (max_size < 10 + m_nbBytes * 2)
      {
         std::cerr<<"hex32line::hex32line(const char* buffer, const int max_size) "
         <<"buffer size too small (< "<<(10 + m_nbBytes*2)<<" bytes )"<<std::endl;
         return;           
      }
      
      m_address = convertHexToUShort(std::string(&buffer[2],4));
      m_type =  convertHexToUChar(std::string(&buffer[6],2));    
      
      //read data
      unsigned int index = 8;
      for (; index < (8 + m_nbBytes * 2); index += 2)
      {
          m_data.push_back(convertHexToUChar(std::string(&buffer[index],2)));
      }
      
      //read checksum
      m_checksum = convertHexToUChar(std::string(&buffer[index],2));
      
      //verify checksum
      verifyChecksum();
      //std::cerr<<"Checksum ok : "<<verifyChecksum()<<std::endl;
      
 }
    
 bool hex32line::operator< (const hex32line &line) const
 {
  
      //This will sort type 0 to be first in the list
      //other types are not important and will be sorted
      //by types instead of addresses
      if (m_type == 0 && line.getType() == 0)
      {
         if (m_address < line.getAddress()) return true;
         else return false;           
      }
      else if (m_type == 0)
      {
           return true;     
      }
      else {
           if (m_type < line.getType()) return true;
           else return false;     
      }                
 }   
    
 //Copy constructor          
 hex32line::hex32line(const hex32line &cpy)
 {
    m_nbBytes = cpy.m_nbBytes;
    m_address = cpy.m_address;
    m_type = cpy.m_type;
    m_data = cpy.m_data;
    m_checksum = cpy.m_checksum;
 }   
 
 unsigned char hex32line::operator [] (int pos)
 {
          if (pos < m_data.size())
          {
                  return m_data[pos];        
          }                  
          return 0xFF;                       
 }
 
 std::vector<unsigned char> & hex32line::getData()
 {
     return m_data;                     
 }
 
 void hex32line::print(std::ostream &out)
 {              
      out<<"nbytes: "<<m_nbBytes<<" ";              
      out<<sethex;
      out<<"address: "<<m_address<<" ";
      out<<"type: "<<m_type<<" ";
      out<<"data: ";
      for (unsigned int i =0; i < m_data.size(); i++)
      {
          out<<(unsigned int) m_data[i]<<" ";    
      }
      out<<"checksum: "<<(unsigned int) m_checksum<<std::endl;
      out<<resethex;
      
 }
 
 size_t hex32line::size() const
 {
        return m_data.size();       
 }
  
 unsigned char hex32line::getNbBytes() const 
 {
          return m_nbBytes;         
 }
 
 unsigned int hex32line::getAddress() const
 {
          return m_address;         
 }
 
 void hex32line::setAddress(unsigned int address)
 {
      m_address = address;
 }
    
 unsigned char hex32line::getType() const
 {         
         return m_type;
 }
 
 unsigned char hex32line::getChecksum()
 {
          return m_checksum;         
 }
 
 bool hex32line::verifyChecksum()
 {
      unsigned char sum = 0;
      
      sum += (unsigned char) (m_nbBytes);
      sum += (unsigned char) (m_address >> 8);
      sum += (unsigned char) (m_address);
      sum += (unsigned char) (m_type);
      for (unsigned int i = 0; i < m_data.size(); i++)
      {
           sum += m_data[i];
      }
      //twos complement
      sum = ~sum + 1;     
      
      //std::cerr<<"calculated checksum : "<<sethex<<(unsigned int)sum<<resethex<<std::endl;
      
      return (sum == m_checksum);
 }
    

 hex32line::hex32line()
 {
	   m_nbBytes = 0;
 	   m_address = 0;
       m_type = 0;
       m_checksum = 0xFF;
 }
         
 unsigned short hex32line::convertHexToUShort(const std::string &hexstring, bool swap) 
 {
        if (hexstring.size() == 4)
        {                                                                  
             unsigned short value = 0;
             
             for (unsigned int i = 0; i < hexstring.size(); i++)
             {
                 value = value * 16 + getHexValue(hexstring[i]);          
             }
             
             //swap bytes
             if (swap)
                value = (value << 8) | (value >> 8);
             
             return value;
        }
        else
        {
            std::cerr<<"convertHexToUShort : invalid number of hex characters size :"
            <<hexstring.size()<<" value : "<<hexstring<<std::endl;   
        }                                       
        return 0xFFFF;
 }

 unsigned char hex32line::convertHexToUChar(const std::string &hexstring) 
 {
        if (hexstring.size() == 2)
        {                                                                 
             unsigned char value = 0;
             
             for (unsigned int i = 0; i < hexstring.size(); i++)
             {
                 value = value * 16 + getHexValue(hexstring[i]);         
             }                                          
             return value;
        }
        else
        {
            std::cerr<<"convertHexToUChar : invalid number of hex characters size :"
            <<hexstring.size()<<" value : "<<hexstring<<std::endl;   
        }                                       
        return 0xFF;
 }

 
         
 hex32doc::hex32doc()
 {
 }
  
 hex32doc::hex32doc(const std::string &filename)
 {
      load(filename);               
 }
  
 void hex32doc::print(std::ostream &out)
 {
      for (unsigned int i = 0; i < m_lines.size(); i++)
      {
          m_lines[i].print(out);
      }  
 }
  
 void hex32doc::clear()
 {
      last_msb = 0;
      m_lines.clear();
 }
  
 void hex32doc::load(const std::string &filename)
 {
      char buffer[1024]; //1K buffer       
      std::ifstream input(filename.c_str());
      while(!input.eof())
      {                                        
          //std::cerr<<"Reading line "<<m_lines.size()<<std::endl;
          
         //read until we get ':'
         input.getline(buffer,1024,':');
         
         //get the whole line
         input.getline(buffer,1024,'\n');
          
         m_lines.push_back(hex32line(buffer,1024));
         
         if (m_lines.back().getType() == 0x01)
            break;
         //system("pause");
      }
      input.close();
      std::cerr<<"Document contains "<<m_lines.size()<<" lines."<<std::endl;
                                 
 }
          
 void hex32doc::parse()
 {
      unsigned int full_address = 0;              
      unsigned short address_high = 0;
      
      for (unsigned int i = 0; i < m_lines.size(); i++)
      {
          
           if (!m_lines[i].verifyChecksum())
           {
                std::cerr<<"Checksum error on line : "<<i<<std::endl;
                m_lines[i].print(std::cerr);
                std::cerr<<"aborting"<<std::endl;
                break;                           
           }
          
           //extended address?
           if (m_lines[i].getType() == 0x04)
           {
              if (m_lines[i].size() == 2)
              {                             
                     address_high = ((unsigned short) (m_lines[i][0]) << 8) | (unsigned short)(m_lines[i][1]);                                                          
              }
              else
              {
                  std::cerr<<"Unknown type 0x04 format"<<std::endl;
                  break;    
              }                           
           }
           else if (m_lines[i].getType() == 0x00) //normal address
           {
                full_address = address_high;
                full_address <<= 16;
                full_address |= (unsigned int) m_lines[i].getAddress();    
                full_address >>= 1; // div 2 for real hardware address                        
                                       
                //modify line to store full address
                m_lines[i].setAddress(full_address);
 
           }
           else if (m_lines[i].getType() == 0x01) //end document
           {
                std::cerr<<"end document found!"<<std::endl;     
           }                  
      }
      
      //sort lines
      //sort_lines();
   
      //remove non normal address lines
        
 }
  
 size_t hex32doc::size()
 {
        return m_lines.size();       
 }
  
 hex32line& hex32doc::getLine(unsigned int no)
 {
           //Q_ASSERT(no < m_lines.size());
           return m_lines[no];          
 }        
         
 void hex32doc::sort_lines()
 {
        sort(m_lines.begin(), m_lines.end());   
 }


 bool hex32doc::validate()
 {
     for (unsigned int i = 0; i < m_lines.size(); i++)
     {
         if (!m_lines[i].verifyChecksum())
         {
             return false;
         }
     }
     return true;
 }
}//namespace hexutils


