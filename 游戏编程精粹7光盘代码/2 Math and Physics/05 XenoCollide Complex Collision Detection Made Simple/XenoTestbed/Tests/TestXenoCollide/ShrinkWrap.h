
#pragma once

class CollideGeometry;
class HullMaker;

// This method is used to convert a support mapping (CollideGeometry) into
// a set of points on the surface of the shape.  These points are then fed
// to the provided HullMaker to generate a set of faces that can be used
// to draw the collision shape.
void ShrinkWrap(HullMaker* hullMaker, CollideGeometry& g, int32 generationThreshold = -1);

