#ifndef _INSTANCE_
#define _INSTANCE_

#include "Primitive.h"
#include "Vector.h"
#include "Matrix3.h"
#include "AABBox.h"
#include "HitRecord.h"

/*class Instance : public Primitive
{
public:

	Instance(Material*mat, Object*obj, Matrix3 rotate, Vector translate) :
		Primitive(mat), mObj(obj), mRotate(rotate), mTranslate(translate){ }

	virtual void preprocess(){ 
		mRotateInverse = inverse(mRotate);

		AABBox objBox;
		mObj->getAABBox(objBox);

		for (int i = 0; i < 8; i++){
			mAABBox += (mRotate*objBox.corner(i)) + mTranslate;
		}	
	}
		
	virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray)const
	{
		Ray r = ray;
		HitRecord subhit;
		Vector tuna1(ray.org - mTranslate);
		Point tuna2(mRotateInverse*tuna1);
		r.org = tuna2;
		r.dir = mRotateInverse*ray.dir;
		r.dir.normalize();
		r.init();


		mObj->intersect(subhit, context, r);
		if (subhit.getPrimitive()){
			Vector p(r.org + subhit.minT() * r.dir);

			p = (mRotate*p) + mTranslate;
			Vector tuna3(p - ray.org);

			double distSq = tuna3.length2();
			if ((hit.minT()*hit.minT()) > distSq){
				double dist = sqrt(distSq);
				if (hit.hit(dist, this, mMat)){
					context.getScratchPad()->setPrimitive(subhit.getPrimitive());
				}
			}
		}
	}


	virtual Vector getNormal();

private:
	Object* mObj;	
	Vector mTranslate;
	Matrix3 mRotate;
	Matrix3 mRotateInverse;
	AABBox mAABBox;
};*/


#endif
