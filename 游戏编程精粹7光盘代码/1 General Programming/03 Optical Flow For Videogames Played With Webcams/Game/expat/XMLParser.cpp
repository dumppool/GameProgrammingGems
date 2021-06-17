#pragma warning( disable : 4786 )
// Disable warning for loss of data
#pragma warning( disable : 4244 )  
#pragma warning( disable : 4996 ) // disable deprecated warning 

#include <cstdio>
#include <cassert>
#include <sstream>
#include <fstream>
//#define _STATIC

#include "XMLParser.h"
#include "expat.h"


#ifdef _DEBUG
#pragma comment (lib, "libexpat.lib")
#else
#pragma comment (lib, "libexpat.lib")
#endif


/*------------------------------------------------------------------
| 
\------------------------------------------------------------------*/
std::string decodeFromUTF8(const char *data, size_t nMax) {    
	const unsigned char *szSource = (const unsigned char *) data;

	std::string sFinal;
	sFinal.reserve( nMax );

	size_t n;    
	for (n = 0; n < nMax; ++n) {
		unsigned char z = szSource[n];        
		if (z < 127) {            
			sFinal += (char)z;        
		}        
		else if (z >= 192 && z <= 223)        
		{
			assert( n < nMax - 1);
			// character is two bytes            
			if (n >= nMax - 1)                 
				break; // something is wrong            
			unsigned char y = szSource[n+1];            
			sFinal += (char)( (z-192)*64 + (y-128) );            
			n = n + 1;        
		}
		else if (z >= 224 && z <= 239)        
		{            
			// character is three bytes            
			assert( n < nMax - 2);
			if (n >= nMax - 2)                 
				break; // something is wrong            
			unsigned char y = szSource[n+1];            
			unsigned char x = szSource[n+2];            
			sFinal += (char)( (z-224)*4096 + (y-128)*64 + (x-128) );            
			n = n + 2;        
		}        
		else if (z >= 240 && z <= 247)        
		{            
			// character is four bytes            
			assert( n < nMax - 3);
			if (n >= nMax - 3)                 
				break; // something is wrong            
			unsigned char y = szSource[n+1];            
			unsigned char x = szSource[n+2];            
			unsigned char w = szSource[n+3];            
			sFinal += (char)( (z-240)*262144 + (y-128)*4096 +
				(x-128)*64 + (w-128) );            
			n = n + 3;        
		}
    else if (z >= 248 && z <= 251)        
		{            
			// character is four bytes            
			assert( n < nMax - 4);
			if (n >= nMax - 4)                 
				break; // something is wrong            
			unsigned char y = szSource[n+1];            
			unsigned char x = szSource[n+2];            
			unsigned char w = szSource[n+3];            
			unsigned char v = szSource[n+4];            
			sFinal += (char)( (z-248)*16777216 + (y-128)*262144 + 
				(x-128)*4096 + (w-128)*64 + (v-128) );            
			n = n + 4;        
		}        
		else if (z >= 252 && z <= 253)       
		{            
			// character is five bytes            
			assert( n < nMax - 5);
			if (n >= nMax - 5)                 
				break; // something is wrong            
			unsigned char y = szSource[n+1];            
			unsigned char x = szSource[n+2];            
			unsigned char w = szSource[n+3];            
			unsigned char v = szSource[n+4];            
			unsigned char u = szSource[n+5];            
			sFinal += (char)( (z-252)*1073741824 + (y-128)*16777216 +
				(x-128)*262144 + (w-128)*4096 + (v-128)*64 + (u-128) );
            n = n + 5;        
		}    
	}    
	return sFinal;
}

/*------------------------------------------------------------------
| 
\------------------------------------------------------------------*/
std::string MKeyValue::getString(const std::string &what, const std::string default_value) const {
	const_iterator it = find (what);
	if (it == end())
		return default_value;
	return decodeFromUTF8 ( it->second.c_str(), strlen ( it->second.c_str() ) );
}

/*------------------------------------------------------------------
| 
\------------------------------------------------------------------*/
int    MKeyValue::getInt (const char *what, int default_value) const {
	const_iterator it = find (what);

	if (it == end())
		return default_value;
	return atoi (it->second.c_str());
}

template < class T >
void putKey( MKeyValue &k, const char *what, T value ) {
	/*std::ostrstream oss;
	oss << value << '\0';
  char *buf = oss.str( );
	k[ what ] = buf;
  delete buf;*/
	// Nos libramos del delete?
	std::ostringstream oss;
	oss << value;
	const std::string &buf = oss.str( );
	k[ what ] = buf;
}

void MKeyValue::put( const char *what, int value ) {
	putKey ( *this, what, value);
}
void MKeyValue::put( const char *what, bool value ) {
	putKey ( *this, what, value);
}
void MKeyValue::put( const char *what, float value ) {
	putKey ( *this, what, value);
}
/*
void MKeyValue::put( const char *what, const TPoint &value ) {
	putKey ( *this, what, value);
}
void MKeyValue::put( const char *what, const TColor &value ) {
	char tmp[512];
  snprintf (tmp, sizeof(tmp)-1, "%f %f %f %f", value.r, value.g, value.b, value.a);
  (*this)[ what ] = tmp;
}
*/
void MKeyValue::put( const char *what, const char *value ) {
	(*this)[ what ] = std::string( value );
}

float MKeyValue::getFloat (const char *what, float default_value) const {
	const_iterator it = find (what);

	if (it == end())
		return default_value;
	return atof (it->second.c_str());
}

bool MKeyValue::getBool (const char *what, bool default_value) const {
	const_iterator it = find (what);
	if (it == end())
		return default_value;
  // Check agains valid keywords
  const char *yes[] = {"1", "yes", "YES", "Yes", "true", "TRUE", "True"};
  int i = 0;
  for (i=0; i<sizeof (yes)/sizeof(yes[0]); ++i) 
    if (it->second == yes[i])
      return true;
  return false;
}
/*
TPoint MKeyValue::getPoint(const std::string &what) const {
	const_iterator it = find (what);
	if (it == end())
		return TPoint( 0.0f, 0.0f, 0.0f );

	std::string txt = it->second;
	float x,y,z;
	if (txt == "") 
		return TPoint (0,0,0);
	int n = sscanf (txt.c_str(), "%f %f %f", &x, &y, &z);
	assert (n == 3);
	return TPoint (x,y,z);
}

TColor MKeyValue::getColor( const std::string &what ) const {
	const_iterator it = find( what );
	if( it == end( ) || what == "" )
		return TColor( 0.0f, 0.0f, 0.0f, 1.0f );

	std::string txt = ( *it ).second;

	float r, g, b, a;
	int n = sscanf( txt.c_str( ), "%f %f %f %f", &r, &g, &b, &a );

	assert( n == 3 || n == 4 );

	if( n == 3 )
		a = 1.0f;

	return TColor( r, g, b, a );
}

	*/

void MKeyValue::writeSingle (std::ostream &os, const char *what) const {
	os << "<" << what << "\n";
  writeAttributes ( os );
	os << "\t/>\n";
}

void MKeyValue::writeAttributes ( std::ostream &os ) const {
  const_iterator i = begin( );
	while( i != end( ) ) {
		os << "\t" << i->first << "=\"" << i->second << "\"\n";
		++i;
	}
}

void MKeyValue::writeStartElement( std::ostream &os, const char *what) const {
	os << "<" << what << "\n";
  writeAttributes( os );
  os << ">\n";
}

void MKeyValue::writeEndElement( std::ostream &os, const char *what) const {
	os << "</" << what << ">\n";
}

/*-<==>-----------------------------------------------------------------
/ Real xml handlers
/----------------------------------------------------------------------*/
CXMLParser::CXMLParser( )
: abort_parse( false )
, notify_missing_files( true )
{
}

void CXMLParser::onStartElement (const std::string &elem, MKeyValue &atts) {
}

void CXMLParser::onData (const std::string &data) {
}

void CXMLParser::onEndElement (const std::string &elem) {
}

/*------------------------------------------------------------------
| 
\------------------------------------------------------------------*/
void CXMLParser::xmlStartElement (void *userData, const char *element, const char **atts) {
  CXMLParser *self = (CXMLParser *)userData;

	if( self->abort_parse )
		return;

	// Convert the array of char pointers to a map
	MKeyValue map_atts;
	for (const char **p = atts; *p; p += 2) {
		const char *key = *p;
		const char *value = *(p+1);
		map_atts[key] = value; 
  } 
  self->onStartElement(std::string (element), map_atts);
}

void CXMLParser::xmlData( void *userData, const char *data, int length ) {
	CXMLParser *self = (CXMLParser *)userData;	

	if( self->abort_parse )
		return;

	self->onData( decodeFromUTF8 (data, length ) );
}

void CXMLParser::xmlEndElement (void *userData, const char *element) {
	CXMLParser *self = (CXMLParser *)userData;

	if( self->abort_parse )
		return;

	self->onEndElement(std::string (element));
}

/*------------------------------------------------------------------
| 
\------------------------------------------------------------------*/
void CXMLParser::xmlAbortParse( ) {
	abort_parse = true;
}

/*------------------------------------------------------------------
| 
\------------------------------------------------------------------*/
bool CXMLParser::xmlParseFile (const std::string &filename) {
	std::ifstream is( filename.c_str() );
	if ( !is.is_open() ) {
		xml_error = "XML File " + filename + " not found";
		if( notify_missing_files ) {
			assert( !"Missing XML File");
			//warn( "%s", xml_error.c_str() );
		}
		return false;
	}
	return xmlParseStream( is, filename.c_str() );
}

/*------------------------------------------------------------------
| 
\------------------------------------------------------------------*/
bool CXMLParser::xmlParseStream (std::istream &is, const char *stream_name) {

	const int buffer_size = 8192;
  char buf[ buffer_size ];

	// Clear error msg
	xml_error = "";
	abort_parse = false;

  XML_Parser parser = XML_ParserCreate(NULL);

  XML_SetUserData(parser, this);
  XML_SetElementHandler(parser, xmlStartElement, xmlEndElement);
	XML_SetCharacterDataHandler(parser, xmlData);

  bool done = false;
  do {
    is.read (buf, sizeof(buf));
		size_t len = is.gcount( );
    if (!XML_Parse(parser, buf,(int) len, done)) {
			char msg[512];
			printf (msg, sizeof(msg)-1, "XML Parser error '%s' at line %d when processing input stream( %s )\n",
          XML_ErrorString(XML_GetErrorCode(parser)),
          XML_GetCurrentLineNumber(parser),
					stream_name ? stream_name : "<UNNAMED>"
          );
			xml_error = std::string (msg);
			done = false;
			break;
		} else {
			// A request to abort is not considered as an error
			done = (len < sizeof(buf)) || abort_parse;
		}

  } while (!done);

  XML_ParserFree(parser);

	// Notify xml parse errors
	if( !done ) {
		char *error=strdup(xml_error.c_str());
		assert( !"XML Parse Error" );
		//warn( "%s", xml_error.c_str() );
	}

	return done;
}

