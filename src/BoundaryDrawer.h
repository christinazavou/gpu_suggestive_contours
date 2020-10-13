/*
 * BoundaryDrawer.h
 *
 *      Author: Christina Zavou
 */

#ifndef BOUNDARYDRAWER_H_
#define BOUNDARYDRAWER_H_

#include "Drawer.h"
#include "LineDrawer.h"

class BoundaryDrawer: public LineDrawer {

public:
    BoundaryDrawer(trimesh::vec color, float linewidth);
    virtual ~BoundaryDrawer();
    virtual void draw(Model* m, trimesh::vec camera_position);

};

#endif /* BOUNDARYDRAWER_H_ */
