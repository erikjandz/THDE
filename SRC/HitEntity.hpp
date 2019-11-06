//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef HIT_ENTITY_HPP
#define HIT_ENTITY_HPP

/// @file
/// \brief
/// Class for hit entity
/// \details
// This class serves as storage entity for the player's hits by other players
class HitEntity final
{
public:
	/// \brief
	/// Default constructor
	/// \details
    // Default constructor for array initialization
	 HitEntity() {}

	/// \brief
	/// Constructor
	/// \details
    // Constructor needs initial values
    HitEntity(int playerID, int weaponPower):
      _playerID(playerID),
      _weaponPower(weaponPower)
      {

      }

	/// \brief
	/// Get playerID function
	/// \details
	// This function returns the playerID
	int getPlayerID()
	{
	  return _playerID;
	}

	/// \brief
	/// Get weaponPower function
	/// \details
	// This function returns the weaponPower
	int getWeaponPower()
	{
	  return _weaponPower;
	}

private:
    int _playerID;
    int _weaponPower;
};

#endif
