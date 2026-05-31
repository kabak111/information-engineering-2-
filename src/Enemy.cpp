#include "Enemy.h"
#include "Game.h"
#include "MathUtils.h"
#include <cmath>
Enemy::Enemy(sf::Texture& texture,const sf::Vector2f& pos,float eHp,float eSpeed,int dmg){ sprite.setTexture(texture); sprite.setOrigin(texture.getSize().x/2.f,texture.getSize().y/2.f); sprite.setScale(1.6f,1.6f); sprite.setPosition(pos); hp=eHp; speed=eSpeed; contactDamage=dmg; animationTimer=0.f; }
void Enemy::update(Game& game,float dt){ Player* p=game.getPlayer(); if(!p)return; sf::Vector2f dir=normalize(p->getPosition()-sprite.getPosition()); sprite.move(dir*speed*dt); animationTimer+=dt; float pulse=1.6f+std::sin(animationTimer*6.f)*0.05f; sprite.setScale(pulse,pulse); }
void Enemy::draw(sf::RenderWindow& window){ window.draw(sprite); }
sf::Vector2f Enemy::getPosition() const{return sprite.getPosition();} float Enemy::getRadius() const{return 24.f;} void Enemy::takeDamage(float amount){ hp-=amount; if(hp<=0.f) destroy(); } int Enemy::getContactDamage() const{return contactDamage;}
MeleeEnemy::MeleeEnemy(sf::Texture& t,const sf::Vector2f& p,int wave):Enemy(t,p,3.f+wave*0.6f,110.f+wave*5.f,1){}
FastEnemy::FastEnemy(sf::Texture& t,const sf::Vector2f& p,int wave):Enemy(t,p,2.f+wave*0.4f,175.f+wave*8.f,1){}
RangedEnemy::RangedEnemy(sf::Texture& t,const sf::Vector2f& p,int wave):Enemy(t,p,4.f+wave*0.5f,85.f+wave*3.f,1){ shootTimer=1.4f; }
void RangedEnemy::update(Game& game,float dt){ Player* p=game.getPlayer(); if(!p)return; sf::Vector2f dir=normalize(p->getPosition()-sprite.getPosition()); float d=distance(p->getPosition(),sprite.getPosition()); if(d>300.f) sprite.move(dir*speed*dt); shootTimer-=dt; if(shootTimer<=0.f && d<550.f){ game.enemyShoot(sprite.getPosition(),dir); shootTimer=1.7f; } animationTimer+=dt; float pulse=1.55f+std::sin(animationTimer*4.5f)*0.04f; sprite.setScale(pulse,pulse); }
