/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

/// \addtogroup clanSqlite_System clanSqlite System
/// \{


#pragma once


#include "sqlite3.h"
#include "API/Database/db_transaction.h"
#include "API/Database/db_transaction_provider.h"

class CL_SqliteConnectionProvider;

/// \brief Sqlite database transaction provider.
class CL_SqliteTransactionProvider : public CL_DBTransactionProvider
{
/// \name Construction
/// \{
public:
	CL_SqliteTransactionProvider(CL_SqliteConnectionProvider *connection, CL_DBTransaction::Type type);
	~CL_SqliteTransactionProvider();
/// \}

/// \name Attributes
/// \{
public:
/// \}

/// \name Operations
/// \{
public:
	void commit();
	void rollback();
/// \}

/// \name Implementation
/// \{
private:
	CL_SqliteConnectionProvider *connection;
	CL_DBTransaction::Type type;

	friend class CL_SqliteConnectionProvider;
/// \}
};


/// \}
