/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

/// \addtogroup clanCore_I_O_Data clanCore I/O Data
/// \{

#pragma once

#include "../api_core.h"

/// \brief Endianess management class.
///
///    <p>The CL_Endian class is used to convert between different endianess. There
///    are also a set of defines to the lazy developer:</p>
///    <p>#define SWAP_IF_BIG(i)<br>
///    #define SWAP_IF_LITTLE(i)<br>
///    #define SWAP_IF_BIG_ALOT(i, times)<br>
///    #define SWAP_IF_LITTLE_ALOT(i, times)<br>
///    #define IS_SYSTEM_64BIT()</p>
///    <p>Each of these defines call CL_Endian::is_system_big() and CL_Endian::swap.</p> 
/// \xmlonly !group=Core/IO Data! !header=core.h! \endxmlonly
class CL_API_CORE CL_Endian
{
public:
/// \name Operations
/// \{

	/// \brief Swaps larger amounts of data between little and big endian.
	///
	/// \param data Data to be swapped.
	/// \param type_size Size of datatype to be swapped. (Must be an even number)
	/// \param total_times Number of 'type_size' size data chunks to be swapped.
	static void swap(void *data, int type_size, int total_times=1);

/// \}
/// \name Attributes
/// \{

	/// \brief Returns true if big endian system.
	/** \return True if big endian system, false otherwise.*/
	static bool is_system_big();

	/// \brief Returns true if 64 bit system.
	/** \return True if 64 bit, false otherwise.*/
	static bool is_system_64bit();
/// \}
};

// Defines to the lazy developer:
#define SWAP_IF_BIG(i) if (CL_Endian::is_system_big()) CL_Endian::swap(&i, sizeof(i))
#define SWAP_IF_LITTLE(i) if (!CL_Endian::is_system_big()) CL_Endian::swap(&i, sizeof(i))

#define SWAP_IF_BIG_ALOT(i, times) if (CL_Endian::is_system_big()) CL_Endian::swap(&i, sizeof(i), times)
#define SWAP_IF_LITTLE_ALOT(i, times) if (!CL_Endian::is_system_big()) CL_Endian::swap(&i, sizeof(i), times)

#define IS_SYSTEM_64BIT() CL_Endian::is_system_64bit()

/// \}
