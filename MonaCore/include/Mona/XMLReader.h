/* 
	Copyright 2013 Mona - mathieu.poux[a]gmail.com
 
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License received along this program for more
	details (or else see http://www.gnu.org/licenses/).

	This file is a part of Mona.
*/

#pragma once

#include "Mona/Mona.h"
#include "Mona/DataReader.h"
#include "Mona/Time.h"


namespace Mona {

class XMLReader : public DataReader, virtual Object {
public:
	XMLReader(PacketReader& packet);

	virtual std::string&		readString(std::string& value);
	virtual double				readNumber();
	virtual bool				readBoolean();
	virtual Time&				readTime(Time& time);
	virtual void				readNull() { packet.next(4); }
	
	bool						readArray(UInt32& size);
	virtual bool				readObject(std::string& type,bool& external);
	virtual Type				readItem(std::string& name);
	virtual Type				followingType();

	virtual void				reset();

private:
	const UInt8*	readBytes(UInt32& size);

	/// \brief ignore comment and return next type
	Type			parseComment(const UInt8* first, const UInt8* cur);

	/// \brief remove last tag added and return type
	Type			removeTag();
	
	/// \brief parse a primitive value : string (with/without quotes), data, number
	/// and return type
	Type			parsePrimitive(const UInt8* cur);

	/// \brief ignore spaces and get current char
	const UInt8*	current();

	/// \brief Intern structure for ordering tags and subtags
	/// and return the good type in followingType()
	class TagXML : virtual Object {
	public:
		TagXML(const std::string& name) : tagName(name), arrayStarted(false) {}
		virtual ~TagXML() {}

		bool		arrayStarted;
		std::string tagName;
		std::string currentSubTag;
	};

	enum ReadStep {
		NOTHING=0,
		POP_TAG,
		ADD_TAG,
		START_ARRAY
	};

    std::deque<TagXML>		_queueTags; ///< record each tags and the is last subtag
	ReadStep				_nextStep; ///< if there is an action to terminate
	UInt32			        _pos;
	std::string		        _text;
	Time			        _date;
	Type			        _last;
	double			        _dval;
	bool                    _tagOpened;
	bool					_objectPrimitive;
	bool					_inited; ///< to return an array as first type
};


} // namespace Mona