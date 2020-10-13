/*
 * Author: Christina Zavou
 */

#include "BoundaryDrawer.h"
#include "mesh_info.h"

BoundaryDrawer::BoundaryDrawer(trimesh::vec color = trimesh::vec(0,0,0), float linewidth = 4.0): LineDrawer(color,linewidth)
{
    //nothing left to do
}
void BoundaryDrawer::draw(Model* m, trimesh::vec camera_position){
	if(isVisible()){
		// Setup OpenGL to draw nice lines
		glPolygonOffset(5.0f, 30.0f);
		glEnable(GL_LINE_SMOOTH); // line anti-aliasing
		glDisable(GL_LIGHTING);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// set color and linewidth_
		glLineWidth(linewidth_);
		glColor3f(linecolor_[0],linecolor_[1],linecolor_[2]);

        glBegin(GL_LINES);
        for (size_t i = 0; i < m->mesh_->faces.size(); i++) {
            for (int j = 0; j < 3; j++) {
                if (m->mesh_->across_edge[i][j] >= 0)
                    continue;
                int v1 = m->mesh_->faces[i][(j+1)%3];
                int v2 = m->mesh_->faces[i][(j+2)%3];
                glVertex3fv(m->mesh_->vertices[v1]);
                glVertex3fv(m->mesh_->vertices[v2]);
            }
        }
        glEnd();
	}
}

BoundaryDrawer::~BoundaryDrawer(){
    // nothing to do in destructor
}
