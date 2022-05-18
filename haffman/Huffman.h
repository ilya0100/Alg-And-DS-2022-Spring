#include <vector>
#include <iostream>


class IInputStream {

};

class IOutputStream {

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Входной поток
class CInputStream : public IInputStream {
public:
	CInputStream( const std::vector<unsigned char>& _body ) : index( 0 ), body( _body ) {}

	virtual bool Read( unsigned char& value );

private:
	unsigned int index;
	const std::vector<unsigned char>& body;
};

bool CInputStream::Read( unsigned char& value )
{
	if( index >= body.size() ) {
		return false;
	}

	value = body[index++];
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Выходной поток
class COutputStream : public IOutputStream {
public:
	COutputStream( std::vector<unsigned char>& _body ) : body( _body ) { body.clear(); }
	virtual void Write( unsigned char value ) { body.push_back( value ); }

private:
	std::vector<unsigned char>& body;
};
