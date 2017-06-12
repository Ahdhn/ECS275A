#ifndef _SCRATCHPAD_
#define _SCRATCHPAD_

class Primitive;
class ScratchPad
{
public:
	ScratchPad() : mPrimitive(NULL) {  }
	Primitive* getPrimitive() { return mPrimitive; }
	void setPrimitive(Primitive* prim) { mPrimitive = prim; }

private:
	Primitive*mPrimitive;

};

#endif