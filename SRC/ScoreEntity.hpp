#ifndef SCOREENTITY_HPP
#define SCOREENTITY_HPP

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

#endif
