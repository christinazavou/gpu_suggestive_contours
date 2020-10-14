/**
 * A simple GLUT-based OpenGL viewer.
 * (Based on the TriMesh2 library code where indicated)
 * author: Jeroen Baert
 */

#define GLEW_STATIC

// GLEW
#include <GL/glew.h> // OpenGL Extension Wrangler functions
// FROM OS
#include <GL/gl.h> // OpenGL functions itself

// TRIMESH2
#include <TriMesh.h>
#include <XForm.h>
#include <GLCamera.h>
#include <GL/glut.h> // FreeGlut window management

#include <string>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <sys/stat.h>


// SELFMADE
#include "../Model.h"
#include "../BaseDrawer.h"
#include "../EdgeContourDrawer.h"
#include "../FaceContourDrawer.h"
#include "../SuggestiveContourDrawer.h"
#include "../BoundaryDrawer.h"

using std::string;

// Global variables (if this Viewer were a class, this would be its attributes)
std::vector<Model*> models; // the model list
std::vector<trimesh::xform> transformations; // model transformations
trimesh::TriMesh::BSphere global_bsph; // global boundingbox
trimesh::xform global_transf; // global transformations
trimesh::GLCamera camera; // global camera

#define PI 3.14159265
trimesh::vec camera_pos;

// The drawers we'll use in this demo
BaseDrawer* b;
EdgeContourDrawer* b1;
SuggestiveContourDrawer* b2;
BoundaryDrawer* b3;

// toggle for diffuse lighting
bool diffuse = false;

/**
 * Clears the OpenGL Draw and Depth buffer, resets all relevant OpenGL states
 */
void cls(){
	glDisable(GL_DITHER);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_NORMALIZE);
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);
	glDisable(GL_COLOR_MATERIAL);
	glClearColor(1, 1, 1, 0);;
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * Update the global bounding sphere
 * (from TriMesh2 library)
 */
void update_boundingsphere(){
	// largest box possible
	trimesh::point boxmin(1e38, 1e38, 1e38);
	trimesh::point boxmax(-1e38, -1e38, -1e38);
	// find outer coords
	for (unsigned int i = 0; i < models.size(); i++){
		trimesh::point c = transformations[i] * models[i]->mesh_->bsphere.center;
		float r = models[i]->mesh_->bsphere.r;
		for (int j = 0; j < 3; j++) {
			boxmin[j] = std::min(boxmin[j], c[j]-r);
			boxmax[j] = std::max(boxmax[j], c[j]+r);
		}
	}
	trimesh::point &gc = global_bsph.center;
	float &gr = global_bsph.r;
	gc = 0.5f * (boxmin + boxmax);
	gr = 0.0f;
	// find largest possible radius for sphere
	for (unsigned int i = 0; i < models.size(); i++) {
		trimesh::point c = transformations[i] * models[i]->mesh_->bsphere.center;
		float r = models[i]->mesh_->bsphere.r;
		gr = std::max(gr, dist(c, gc) + r);
	}
}

/**
 * Reset the current view: undo all camera transformations.
 */
void resetview()
{
	// kill the cam
	camera.stopspin();
	// undo all model transformations
	for (unsigned int i = 0; i < models.size(); i++){
		transformations[i] = trimesh::xform();
	}
	// recompute bounding sphere
	update_boundingsphere();
	// put ourselves in middle
	global_transf = trimesh::xform::trans(0, 0, -5.0f * global_bsph.r)
	        * trimesh::xform::trans(-global_bsph.center);
}

/**
 * Setup the OpenGL lighting
 */
void setup_lighting(){
	if(!diffuse){
		trimesh::Color c(1.0f);
		glColor3fv(c);
		glDisable(GL_LIGHTING);
	}
	else{
		GLfloat light0_diffuse[] = { 0.85, 0.85, 0.85, 0.85 };
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_NORMALIZE);
	}
}


/**
 * Save the current image to a PPM file.
 * (from TriMesh2 library)
 */
void dump_image(string model_dir, int view_num)
{
    // Find first non-used filename
    const char filenamepattern[] = "%s/%d.ppm";
    FILE *f;
    while (1) {
        char filename[1024];
        sprintf(filename, filenamepattern, model_dir.c_str(), view_num);
        f = fopen(filename, "rb");
        if (!f) {
            f = fopen(filename, "wb");
            printf("\n\nSaving image %s... ", filename);
            fflush(stdout);
            break;
        }
        fclose(f);
    }

    // Read pixels
    GLint V[4];
    glGetIntegerv(GL_VIEWPORT, V);
    GLint width = V[2], height = V[3];
    char *buf = new char[width*height*3];
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(V[0], V[1], width, height, GL_RGB, GL_UNSIGNED_BYTE, buf);

    // Flip top-to-bottom
    for (int i = 0; i < height/2; i++) {
        char *row1 = buf + 3 * width * i;
        char *row2 = buf + 3 * width * (height - 1 - i);
        for (int j = 0; j < 3 * width; j++)
            std::swap(row1[j], row2[j]);
    }

    // Write out file
    fprintf(f, "P6\n#\n%d %d\n255\n", width, height);
    fwrite(buf, width*height*3, 1, f);
    fclose(f);
    delete [] buf;

    printf("Done.\n\n");
}

/**
 * Reposition the camera and draw every model in the scene.
 */
void redraw(const char* out_dir, int angle_y){
	// setup camera and push global transformations
	camera.setupGL(global_transf * global_bsph.center, global_bsph.r);
    glPushMatrix();
	glMultMatrixd(global_transf);
    glRotatef(30,1,0,0);
    glRotatef(angle_y,0,1,0);
    cls();

	// enable depth checking and backface culling
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// compute new camera position
	camera_pos = inv(global_transf) * trimesh::point(0,0,0);

	// setup lighting
	setup_lighting();

	// draw every model
	for (unsigned int i = 0; i < models.size(); i++){
		// push model-specific transformations
		glPushMatrix();
		glMultMatrixd(transformations[i]);
		// tell model to execute its drawer stack
		models[i]->draw(camera_pos);
		// pop again
		glPopMatrix();
	}
	// pop global transformations
	glPopMatrix();
	glutSwapBuffers();
	std::stringstream out;
	string s = out.str();
	glutSetWindowTitle(s.c_str());
    dump_image(out_dir, angle_y/30);

}

int mkpath(std::string s,mode_t mode){
    size_t pos=0;
    std::string dir;
    int mdret;

    if(s[s.size()-1]!='/'){
        // force trailing / so we can handle everything in loop
        s+='/';
    }

    while((pos=s.find_first_of('/',pos))!=std::string::npos){
        dir=s.substr(0,pos++);
        if(dir.size()==0) continue; // if leading / first time is 0 length
        if((mdret=mkdir(dir.c_str(),mode)) && errno!=EEXIST){
            return mdret;
        }
    }
    return mdret;
}


int main(int argc, char *argv[]){
	// Initialize GLUT window manager
	glutInitWindowSize(512, 512);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInit(&argc, argv);
	glutCreateWindow("Crytek Object Space Contours Demo");
	glewInit();
	glutHideWindow();

	// construct the Drawers we'll use in this demo
	b = new BaseDrawer();
	b1 = new EdgeContourDrawer(trimesh::vec(0,0,0),3.0);
    b2 = new SuggestiveContourDrawer(trimesh::vec(0,0,0), 2.0, true, 0.001);
    b3 = new BoundaryDrawer(trimesh::vec(0.05,0.05,0.05),2.5);

    string prefix_path = "/media/christina/Elements/ANNFASS_SOLUTION";

    string data_name = "buildnet";
    char* names[] = {
            "MILITARYcastle_mesh2135Marios.ply",
//            "RELIGIOUScathedral_mesh0754Marios.ply",
//            "RESIDENTIALhouse_mesh2302Marios.ply",
//            "RESIDENTIALvilla_mesh3265Marios.ply"
    };

//    string data_name = "annfass_buildings";
//    char* names[] = {
//            "28_Stavrou_Economou_Building_01Marios.ply",
//            "29_Lefkaritis_Building_01Marios.ply",
//            "30_Nicolaou_Building_01Marios.ply"};


    // read models from arguments
	for (int i = 0; i < sizeof(names); i++){
		const string modelname = prefix_path + "/proj_style_data/rtsc_in/" + data_name + "/" + names[i];
        const string outpath = prefix_path + "/proj_style_out/rtsc_out/" + data_name + "/" +
                std::string(&names[i][0], &names[i][std::strlen(names[i])-4]);
        mkpath(prefix_path + "/proj_style_out/rtsc_out/" + data_name, 7777);
        mkpath(outpath, 7777);

        // creat model
		Model* m = new Model(modelname.c_str());
		// add drawers to model
		m->pushDrawer(b);
		m->pushDrawer(b1);
		m->pushDrawer(b2);
		m->pushDrawer(b3);
		models.push_back(m);
		// push back blank tranformation matrix
		transformations.push_back(trimesh::xform());

        for (int rot_angle_y = 0; rot_angle_y <= 360; rot_angle_y+=30) {
            resetview();
            redraw(outpath.c_str(), rot_angle_y);
        }

//        models.pop_back();
    }

}

