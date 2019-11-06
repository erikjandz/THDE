//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef HIT_ENTITY_HPP
#define HIT_ENTITY_HPP

// This class serves as storage entity for the player's hits by other players
class HitEntity final
{
public:
    // Default constructor for array initialization
	  HitEntity() {}

    // Constructor needs initial values
    HitEntity(int playerID, int weaponPower):
      _playerID(playerID),
      _weaponPower(weaponPower)
      {

      }

      // This function returns the playerID
      int getPlayerID()
      {
          return _playerID;
      }

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
