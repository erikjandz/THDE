#pragma once

class ScoreEntity{
public:
    ScoreEntity(int _score):
      _score(_score)
      {}

    int getScore(){
        return _score;
    }

    void setScore(int score){
        _score = score;
    }

private:
    int _score;

};
