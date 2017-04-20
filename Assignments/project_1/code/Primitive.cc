
#include "Primitive.h"
#include "HitRecord.h"
#include "Material.h"
#include "Ray.h"
#include "Vector.h"

Primitive::Primitive(Material* matl)
  : matl(matl)
{
}
Primitive::Primitive(Material* matl, bool is_luminous)
	: matl(matl), is_luminous(is_luminous)
{
}
Primitive::~Primitive()
{
}

void Primitive::preprocess()
{
  matl->preprocess();
}

void Primitive::computeUVW(Vector& uvw, const RenderContext& context,
                           const Ray& ray, const HitRecord& hit) const
{
  Point hitpos = ray.origin()+ray.direction()*hit.minT();
  uvw = Vector(hitpos);
}
