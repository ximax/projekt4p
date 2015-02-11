#include "ItemManager.hpp"
inline size_t ItemManager::playerItemCount(){
	return playerItems.size();
}
inline void ItemManager::addItem(Item it){
	playerItems.push_back(it);
}
inline bool ItemManager::deleteItem(const sf::Int32 id)
{
	std::list<Item>::iterator it = playerItems.begin();
	for(;it!=playerItems.end();++it){
		if(it->id == id){
			playerItems.erase(it);
			return 1;
		}
	}
	return 0;
}
Item ItemManager::getItem(const sf::Int32 id) const
{
	for(auto item : playerItems)
	{
		if(item.id == id) return item;
	}
	return Item();
}
void ItemManager::updateItemsData(const sf::Int32 x,const sf::Int32 y)
{
	for(auto &item : playerItems)
	{
		item.x = x;
		item.y = y;
	}
}

