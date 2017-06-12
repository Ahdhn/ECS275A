#ifndef _BHV_
#define _BHV_

#include "Group.h"

class BVH : public Group
{
public:
	BVH() { };
	virtual void preprocess(){
		for (size_t i = 0; i < size(); i++){
			objects[i]->preprocess();
		}
		calcAABBox();
		sortSplit(0);
	}
	virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
	{
		if (mAABBox.intersect1(ray)){
			objects[0]->intersect(hit, context, ray);
			if (objects.size() > 1){
				objects[1]->intersect(hit, context, ray);
			}
		}
	}

	virtual void getAABBox(AABBox&box)const{
		box = mAABBox;
	}

	
private:
	inline size_t size()const{ return objects.size(); }
	inline void clear(){ objects.clear(); }

	void calcAABBox(){
		for (size_t i = 0; i < size(); i++){
			AABBox box;
			objects[i]->getAABBox(box);			
			mAABBox += box;
		}
	}

	void sortSplit(int depth){
		if (objects.size() > 2){
			BVH*left = new BVH();
			BVH*right = new BVH();

			for (size_t i = 0; i < size(); i++){
				if (simpleAxisSort(objects[i], this, depth)){
					left->addObject(objects[i]);
				}
				else{
					right->addObject(objects[i]);
				}
			}
			if (left->size() == 0 || right->size() == 0){
				left->clear();
				right->clear();

				size_t half = objects.size() / 2;

				for (size_t i = 0; i < half; i++){
					left->addObject(objects[i]);
				}
				for (size_t i = half; i < objects.size(); i++){
					right->addObject(objects[i]);
				}
			}

			clear();

			addObject(left);
			left->calcAABBox();
			left->sortSplit(depth + 1);

			addObject(right);
			right->calcAABBox();
			right->sortSplit(depth + 1);
		}
	}


	bool simpleAxisSort(const Object*left, const Object*right, int axis){
		AABBox leftBox, rightBox;
		left->getAABBox(leftBox);
		right->getAABBox(rightBox);
		int idx = axis % 3;
		return leftBox.center()[idx] < rightBox.center()[idx];
	}


	AABBox mAABBox;
};



#endif // !_BHV_
