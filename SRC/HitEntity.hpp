#ifndef HIT_ENTITY_HPP
#define HIT_ENTITY_HPP

class HitEntity final
{
public:
	HitEntity() {}

    HitEntity(int playerID, int weaponPower):
      _playerID(playerID),
      _weaponPower(weaponPower)
      {

      }

    int getPlayerID()
    {
        return _playerID;
    }

    int getWeaponPower()
    {
        return _weaponPower;
    }

private:
    int _playerID;
    int _weaponPower;
};

#endif
