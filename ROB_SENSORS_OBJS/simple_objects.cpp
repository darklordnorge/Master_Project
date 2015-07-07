#include "simple_objects.h"
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>


/* ------------------------------------------------------------ */

SIMPLE_Objects::SIMPLE_Objects( btDynamicsWorld* _world ) : World_Entity ( )
{
  rot.assign    (3, 0.0);
  dim.assign    (3, 0.0);
  colour.assign (3, 0.0);
  world = _world;
}

/* ------------------------------------------------------------ */

void SIMPLE_Objects::set_mass(double _mass){

}

/* ------------------------------------------------------------ */

const vector <double> SIMPLE_Objects::get_pos     ( void ){
    pos[0] = body->getWorldTransform().getOrigin().x();
    pos[1] = body->getWorldTransform().getOrigin().y();
    pos[2] = body->getWorldTransform().getOrigin().z();
    return pos;

}

/* ------------------------------------------------------------ */
/* ------------------------------------------------------------ */

const vector <double> SIMPLE_Objects::get_rot     ( void ){
    btMatrix3x3 rotMatrix = btMatrix3x3(body->getWorldTransform().getRotation());
    float rotX,rotY,rotZ;
    rotMatrix.getEulerZYX(rotZ,rotY,rotX);
    rot[0] = rotX;
    rot[1] = rotY;
    rot[2] = rotZ;
    return rot;
}

/* ------------------------------------------------------------ */

void SIMPLE_Objects::set_rot ( const vector <double> &_rot ){

        btQuaternion rotation;
        rotation.setEulerZYX( 0.0, _rot[1], 0.0);
        btTransform trans = body->getWorldTransform();
        trans.setRotation(rotation);
        body->setWorldTransform(trans);
}

/* ------------------------------------------------------------ */

void SIMPLE_Objects::set_pos ( const vector <double> &_pos ){

     btTransform trans = body->getWorldTransform();
     trans.setOrigin(btVector3(_pos[0],pos[1],_pos[2]));
     body->setWorldTransform(trans);
}
/* ------------------------------------------------------------- */
/* PLANE   PLANE   PLANE   PLANE   PLANE   PLANE   PLANE   PLANE */
/* ------------------------------------------------------------ */
SIMPLE_Plane::SIMPLE_Plane( btDynamicsWorld* world ) : SIMPLE_Objects( world )
{
    type_id = 20;
    addPlane();
}

/* ------------------------------------------------------------- */
void SIMPLE_Plane::addPlane()
{
    btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(5.0),btScalar(0.25),btScalar(5.0)));
    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0.0,-0.25,0.0));
    mass = 0.0;
    btVector3 Inertia(0.0,0.0,0.0);
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo info(mass,groundMotionState,groundShape,Inertia);
    info.m_restitution = 0.0f;
    info.m_friction = 0.5f;
    //info.m_rollingFriction = 0.1;
    body=new btRigidBody(info);
    world->addRigidBody(body);

 }
/* ------------------------------------------------------------ */
#ifdef _GRAPHICS_

void SIMPLE_Plane::render ( )
{

    if(body->getCollisionShape()->getShapeType()!=BOX_SHAPE_PROXYTYPE)
        return;
    glColor3f(0.2,0.2,0.2);
    btVector3 extent=((btBoxShape*)body->getCollisionShape())->getHalfExtentsWithMargin();
    btTransform t;
    body->getMotionState()->getWorldTransform(t);
    float mat[16];
    t.getOpenGLMatrix(mat);
    glPushMatrix();
    glMultMatrixf(mat);     //translation,rotation
                   glBegin(GL_QUADS);
                           glVertex3f(-extent.x(),extent.y(),-extent.z());
                           glVertex3f(-extent.x(),-extent.y(),-extent.z());
                           glVertex3f(-extent.x(),-extent.y(),extent.z());
                           glVertex3f(-extent.x(),extent.y(),extent.z());
                   glEnd();
                   glBegin(GL_QUADS);
                           glVertex3f(extent.x(),extent.y(),-extent.z());
                           glVertex3f(extent.x(),-extent.y(),-extent.z());
                           glVertex3f(extent.x(),-extent.y(),extent.z());
                           glVertex3f(extent.x(),extent.y(),extent.z());
                   glEnd();
                   glBegin(GL_QUADS);
                           glVertex3f(-extent.x(),extent.y(),extent.z());
                           glVertex3f(-extent.x(),-extent.y(),extent.z());
                           glVertex3f(extent.x(),-extent.y(),extent.z());
                           glVertex3f(extent.x(),extent.y(),extent.z());
                   glEnd();
                   glBegin(GL_QUADS);
                           glVertex3f(-extent.x(),extent.y(),-extent.z());
                           glVertex3f(-extent.x(),-extent.y(),-extent.z());
                           glVertex3f(extent.x(),-extent.y(),-extent.z());
                           glVertex3f(extent.x(),extent.y(),-extent.z());
                   glEnd();
                   glBegin(GL_QUADS);
                           glVertex3f(-extent.x(),extent.y(),-extent.z());
                           glVertex3f(-extent.x(),extent.y(),extent.z());
                           glVertex3f(extent.x(),extent.y(),extent.z());
                           glVertex3f(extent.x(),extent.y(),-extent.z());
                   glEnd();
                   glBegin(GL_QUADS);
                           glVertex3f(-extent.x(),-extent.y(),-extent.z());
                           glVertex3f(-extent.x(),-extent.y(),extent.z());
                           glVertex3f(extent.x(),-extent.y(),extent.z());
                           glVertex3f(extent.x(),-extent.y(),-extent.z());
                   glEnd();
           glPopMatrix();
    glPopMatrix();
}
#endif
/* ------------------------------------------------------------- */
/* BRICK   BRICK   BRICK   BRICK   BRICK   BRICK   BRICK   BRICK */
/* ------------------------------------------------------------- */

SIMPLE_Brick::SIMPLE_Brick(int ind, double *data , btDynamicsWorld* world) : SIMPLE_Objects( world )
{
  index = ind;
  pos.assign    (9, 0.0);
  start_pos.assign(3,0.0);
  //corners.assign (4,0.0);

  pos[0] = start_pos[0] = data[0];//X position
  pos[1] = start_pos[1] = data[1];//Y position
  pos[2] = start_pos[2] = data[2];//Z position

  dim[0] = data[3]; //Length on the X axis
  dim[1] = data[5]; //Height on the Y axis
  dim[2] = data[4]; //Width on the Z axis
  
  rot[1] = data[6]; //Rotation on the XZ plain

  angle = (rot[1]*PI)/180.0;
  
  pos[3] = pos[0] + ((dim[0]*0.5 * cos(angle)));
  pos[4] = pos[2] + ((dim[0]*0.5 * sin(angle)));
  pos[5] = pos[1];


  pos[6] = pos[0] + (-(dim[0]*0.5 * cos(angle)));
  pos[7] = pos[2] + (-(dim[0]*0.5 * sin(angle)));
  pos[8] = pos[1];

  colour[0] = data[7]; //RED
  colour[1] = data[8]; //GREEN
  colour[2] = data[9]; //BLUE
  
  type_id  = 4;
  mass = data[10];
  rotation.setEulerZYX( 0.0, angle, 0.0);
  btTransform t;
  t.setIdentity();
  t.setOrigin(btVector3(pos[0],pos[1]+dim[1]/2.0,pos[2]));
  t.setRotation(rotation);
  btBoxShape* brick=new btBoxShape(btVector3(dim[0]/2.0,dim[1]/2.0,dim[2]/2.0));
  brick->setMargin(0.005);
  btVector3 inertia(0,0,0);
  if(mass!=0.0)
      brick->calculateLocalInertia(mass,inertia);

  btMotionState* motion=new btDefaultMotionState(t);
  btRigidBody::btRigidBodyConstructionInfo info(mass,motion,brick,inertia);
  info.m_restitution = data[11];
  info.m_friction = data[12];
  info.m_rollingFriction = data[13];
  body=new btRigidBody(info);
  body->setDamping(data[14],data[15]);
  world->addRigidBody(body);
 }

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */

void SIMPLE_Brick::reset_pos(){

    btTransform trans = body->getWorldTransform();
    trans.setOrigin(btVector3(start_pos[0],start_pos[1]+dim[1]/2.0,start_pos[2]));
    trans.setRotation(rotation);
    body->setWorldTransform(trans);

}


/* ------------------------------------------------------------ */
#ifdef _GRAPHICS_
void SIMPLE_Brick::render ( )
{
    if(body->getCollisionShape()->getShapeType()!=BOX_SHAPE_PROXYTYPE)
        return;
    glColor3f(colour[0], colour[1], colour[2]);
    btVector3 extent=((btBoxShape*)body->getCollisionShape())->getHalfExtentsWithMargin();
    btTransform t;
    body->getMotionState()->getWorldTransform(t);
    float mat[16];
    t.getOpenGLMatrix(mat);
    glPushMatrix();
    glMultMatrixf(mat);     //translation,rotation
                   glBegin(GL_QUADS);
                           glVertex3f(-extent.x(),extent.y(),-extent.z());
                           glVertex3f(-extent.x(),-extent.y(),-extent.z());
                           glVertex3f(-extent.x(),-extent.y(),extent.z());
                           glVertex3f(-extent.x(),extent.y(),extent.z());
                   glEnd();
                   glBegin(GL_QUADS);
                           glVertex3f(extent.x(),extent.y(),-extent.z());
                           glVertex3f(extent.x(),-extent.y(),-extent.z());
                           glVertex3f(extent.x(),-extent.y(),extent.z());
                           glVertex3f(extent.x(),extent.y(),extent.z());
                   glEnd();
                   glBegin(GL_QUADS);
                           glVertex3f(-extent.x(),extent.y(),extent.z());
                           glVertex3f(-extent.x(),-extent.y(),extent.z());
                           glVertex3f(extent.x(),-extent.y(),extent.z());
                           glVertex3f(extent.x(),extent.y(),extent.z());
                   glEnd();
                   glBegin(GL_QUADS);
                           glVertex3f(-extent.x(),extent.y(),-extent.z());
                           glVertex3f(-extent.x(),-extent.y(),-extent.z());
                           glVertex3f(extent.x(),-extent.y(),-extent.z());
                           glVertex3f(extent.x(),extent.y(),-extent.z());
                   glEnd();
                   glBegin(GL_QUADS);
                           glVertex3f(-extent.x(),extent.y(),-extent.z());
                           glVertex3f(-extent.x(),extent.y(),extent.z());
                           glVertex3f(extent.x(),extent.y(),extent.z());
                           glVertex3f(extent.x(),extent.y(),-extent.z());
                   glEnd();
                   glBegin(GL_QUADS);
                           glVertex3f(-extent.x(),-extent.y(),-extent.z());
                           glVertex3f(-extent.x(),-extent.y(),extent.z());
                           glVertex3f(extent.x(),-extent.y(),extent.z());
                           glVertex3f(extent.x(),-extent.y(),-extent.z());
                   glEnd();
           glPopMatrix();
    glPopMatrix();
}
#endif
/* ------------------------------------------------------------ */
/* CYLINDER    CYLINDER    CYLINDER    CYLINDER    CYLINDER     */
/* ------------------------------------------------------------ */

SIMPLE_Cylinder::SIMPLE_Cylinder(int ind, double *data,  btDynamicsWorld* world ) : SIMPLE_Objects( world  )
{
  index = ind;
  pos.assign    (3, 0.0);
  start_pos.assign(3,0.0);
  pos[0] = start_pos[0] = data[0];//X position
  pos[1] = start_pos[1] = data[1];//Y position
  pos[2] = start_pos[2] = data[2];//Z position
  
  dim[0] = data[4]; //width = dim[0] = radius
  dim[1] = data[5]; //height = dim[1] = height
  dim[2] = data[3];

  rot[1] = data[6]; // no need for rotation to Cylinder
  
  colour[0] = data[7];
  colour[1] = data[8];
  colour[2] = data[9];

  type_id  = 1;
  mass = data[10];
  btTransform t;
  t.setIdentity();
  t.setOrigin(btVector3(pos[0],pos[1]+dim[1]/2.0,pos[2]));
  btCylinderShape* cylinder=new btCylinderShape(btVector3(dim[0]*0.5,dim[1]*0.5,dim[0]*0.5));
  //cylinder->setMargin(dim[1]*0.5);
  btVector3 inertia(0,0,0);
  if(mass!=0.0)
      cylinder->calculateLocalInertia(mass,inertia);

  btMotionState* motion=new btDefaultMotionState(t);
  btRigidBody::btRigidBodyConstructionInfo info(mass,motion,cylinder,inertia);
  info.m_restitution = data[11];
  info.m_friction = data[12];
  info.m_rollingFriction = data[13];
  body=new btRigidBody(info);
  body->setDamping(data[14],data[15]);
  world->addRigidBody(body);
}

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */

void SIMPLE_Cylinder::reset_pos(){

    btTransform trans = body->getWorldTransform();
    trans.setOrigin(btVector3(pos[0],pos[1]+dim[1]/2.0,pos[2]));
    body->setWorldTransform(trans);

}


/* ------------------------------------------------------------ */
#ifdef _GRAPHICS_

void SIMPLE_Cylinder::render ( )
{
    if(body->getCollisionShape()->getShapeType()!=CYLINDER_SHAPE_PROXYTYPE)
        return;
    GLUquadric* quad = gluNewQuadric();
    glColor3f(colour[0], colour[1], colour[2]);
    btVector3 extent=((btCylinderShape*)body->getCollisionShape())->getHalfExtentsWithMargin();
    btTransform t;
    body->getMotionState()->getWorldTransform(t);
    float mat[16];
    t.getOpenGLMatrix(mat);
    glPushMatrix();
    glMultMatrixf(mat);
                  //translation,rotation
                   glTranslatef(0,extent.y(),0);
                   glRotatef(90,1,0,0);
                   gluCylinder(quad,extent.x(),extent.x(),extent.y()*2.0,100, 1);
                   gluDisk(quad, 0, extent.x(), 100, 1);
                   glTranslated(0,0,extent.y()*2.0);
                   gluDisk(quad, 0, extent.x(), 100, 1);

        gluDeleteQuadric(quad);
    glPopMatrix();
}
#endif

/* ------------------------------------------------------------ */
/* SPHERE    SPHERE    SPHERE    SPHERE    SPHERE     SPHERE    */
/* ------------------------------------------------------------ */

SIMPLE_Sphere::SIMPLE_Sphere(int ind, double *data, btDynamicsWorld* world ) : SIMPLE_Objects( world )
{
  index = ind;
  pos.assign    (3, 0.0);
  start_pos.assign(3,0.0);
  pos[0] = start_pos[0] = data[0];//X position
  pos[1] = start_pos[1] = data[1];//Y position
  pos[2] = start_pos[2] = data[2];//Z position

  dim[0] = data[3]; // only radius is required for sphere
  dim[1] = data[4];
  dim[2] = data[5];

  rot[1] = data[6]; // sphere have no orientation for rotation

  colour[0] = data[7];
  colour[1] = data[8];
  colour[2] = data[9];

  type_id  = 2;
  mass = data[10];
  btTransform t;	//position and rotation
  t.setIdentity();
  t.setOrigin(btVector3(pos[0],pos[1]+dim[0],pos[2]));	//put it to x,y,z coordinates
  btSphereShape* sphere=new btSphereShape(dim[0]); //it's a sphere, so use sphereshape
  //sphere->setMargin(dim[0]);
  btVector3 inertia(0,0,0);	//inertia is 0,0,0 for static object, else
  if(mass!=0.0)
      sphere->calculateLocalInertia(mass,inertia);	//it can be determined by this function (for all kind of shapes)

  btMotionState* motion=new btDefaultMotionState(t);	//set the position (and motion)
  btRigidBody::btRigidBodyConstructionInfo info(mass,motion,sphere,inertia);	//create the constructioninfo, you can create multiple bodies with the same info
  info.m_restitution = data[11];
  info.m_friction = data[12];
  info.m_rollingFriction = data[13];
  body=new btRigidBody(info);
  body->setDamping(data[14],data[15]);
  world->addRigidBody(body);	//and let the world know about it
  //body->setLinearVelocity(btVector3(0.0,0.0,0.0));}
}

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */

    void SIMPLE_Sphere::reset_pos(){

        btTransform trans = body->getWorldTransform();
        trans.setOrigin(btVector3(pos[0],pos[1]+dim[0],pos[2]));
        body->setWorldTransform(trans);

    }


    /* ------------------------------------------------------------ */
#ifdef _GRAPHICS_
void SIMPLE_Sphere::render ( )
{
    if(body->getCollisionShape()->getShapeType()!=SPHERE_SHAPE_PROXYTYPE)	//only render, if it's a sphere
        return;
    glColor3f(colour[0], colour[1], colour[2]);
    GLUquadric* quad = gluNewQuadric();
    float r=((btSphereShape*)body->getCollisionShape())->getRadius();
    btTransform t;
    body->getMotionState()->getWorldTransform(t);	//get the transform
    float mat[16];
    t.getOpenGLMatrix(mat);	//OpenGL matrix stores the rotation and orientation
    glPushMatrix();
    glMultMatrixf(mat);	//multiplying the current matrix with it moves the object in place
    gluSphere(quad,r,100,100);
    gluDeleteQuadric(quad);
    glPopMatrix();
}
#endif
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*    LIGHT    LIGHT    LIGHT    LIGHT    LIGHT     LIGHT       */
/* ------------------------------------------------------------ */

SIMPLE_Light::SIMPLE_Light( double *data, btDynamicsWorld* world ) : SIMPLE_Objects( world )
{
  pos.assign    (3, 0.0); 
  pos[0] = data[0];
  pos[1] = data[1];
  pos[2] = data[2];  
  
  dim[0] = data[3];
  dim[1] = data[4];
  dim[2] = data[5];

  rot[1] = data[6];

  colour[0] = data[7];
  colour[1] = data[8];
  colour[2] = data[9];  

  type_id  = 3;
}

/* ------------------------------------------------------------ */

#ifdef _GRAPHICS_
void SIMPLE_Light::render ( ){
  float m[16];
  m[ 0] = 1.0;
  m[ 1] = 0.0;
  m[ 2] = 0.0;
  m[ 3] = 0;
  m[ 4] = 0.0;
  m[ 5] = 1.0;
  m[ 6] = 0.0;
  m[ 7] = 0;
  m[ 8] = 0.0;
  m[ 9] = 0.0;
  m[10] = 1.0;
  m[11] = 0;
  m[12] = pos[0];
  m[13] = pos[1];
  m[14] = pos[2];
  m[15] = 1;
  
  glColor3f(colour[0], colour[1], colour[2]);
  glPushMatrix();
  glMultMatrixf(m);
  glutSolidSphere(dim[0],100,100);
  glPopMatrix();
}
#endif

/* ------------------------------------------------------------ */
/* ------------------------------------------------------------ */
