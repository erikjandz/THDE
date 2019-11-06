//          Copyright Youri de Vor, Erik de Zeeuw, Hugo Cornel, Matthijs Koelewijn 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef SCOREENTITY_HPP
#define SCOREENTITY_HPP

/// \brief
/// Class for score
/// \details
// This class serves as a storage entity for the player's score
class ScoreEntity{
public:
	/// \brief
	/// Constructor
	/// \details
	/// Constructor needs score value
    ScoreEntity(int _score):
      _score(_score)
      {}

	/// \brief
	/// Get score function
	/// \details
    /// Function that returns score
    int getScore(){
        return _score;
    }

	/// \brief
	/// Set score function
	/// \details
	/// Function that sets score
    void setScore(int score){
        _score = score;
    }

private:
    int _score;

};

#endif
