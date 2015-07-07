#include "parameters.h"

bool callbackFunc(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2);

//void myTickCallback(btDynamicsWorld *world, btScalar timeStep);

/* ---------------------------------------- */

void Parameters::read_run_parameter_file( const char *run_name ){
  
  ifstream I ("../MISC/init_run.txt");
  if(!I) {
    cerr << "File for Run Parameters not found" <<endl;
    exit(0);
  }
  
  /* ------------------------------- */
  //Load parameters from init_run.txt
  /* ------------------------------- */
  root_seed         = getInt('=', I);
  num_generations   = getInt('=', I);
  num_evaluations   = getInt('=', I);
  num_iterations    = getInt('=', I);
  num_agents        = getInt('=', I);
  
  dump_stats_every  = getInt('=', I);
  dump_genome_every = getInt('=', I);
  
  getStr('=', I, statsFileDir );
  getStr('=', I, revalFileDir );
  getStr('=', I, genomeFileDir );
  getStr('=', I, type_of_controller );
  getStr('=', I, type_of_ga );
  
  simulation_time_step = getDouble('=', I);
  physics_step = simulation_time_step/6.0;

  num_bricks        = getInt('=', I);
  num_cylinders     = getInt('=', I); 
  num_spheres       = getInt('=', I);
  num_lights        = getInt('=', I);
  num_objects       = num_bricks + num_cylinders +  num_spheres + num_lights;

  int num_obj_properties = getInt('=', I);

  if ( num_objects ){
    objects_data = new double*[num_objects];
    
    for(int b=0; b < num_objects; b++){
      objects_data[b] = new double[num_obj_properties];
    }
    
    for(int c=0; c < num_obj_properties; c++) {
      for(int b=0; b < num_objects; b++) {
  	objects_data[b][c] = getDouble('=', I);
      }
    }
  }
  /* ------------------------------- */
  //End of loading parameters
  /* ------------------------------- */
  
  strcpy(statsFileName, run_name);
  strcpy(revalFileName, run_name);
  strcpy(rootSeedFileDir, statsFileDir );
  strcpy(rootSeedFileName, run_name );
  strcpy(genomeFileName, run_name);  
}


/* ---------------------------------------- */

Parameters::Parameters(const char *_run_name_ ){
  root_seed = 0;
  count = 0;
  run_name = new char[500];
  collision_peer = new int*[100];
  sprintf(run_name, "%s", _run_name_);
  read_run_parameter_file( run_name );//Open the first configuration file called init_run.txt
   init_physics_param( );
}


/* ---------------------------------------- */

void Parameters::setDeltaTime(btScalar deltaTime)
{
/*
    double ms = deltaTime*1000000.;
    double minFPS = 1000000.f/60.f;
    if (ms > minFPS)
        ms = minFPS;

    m_Time += ms;

*/
}

/* ---------------------------------------- */


btScalar Parameters::getDeltaTimeMicroseconds()  
{
/*
#ifdef USE_BT_CLOCK
        btScalar dt = (btScalar)m_clock.getTimeMicroseconds();
        m_clock.reset();
        return dt;
#else
        return btScalar(16666.);
#endif
*/
    }
/* ---------------------------------------- */

void Parameters::myProcessCallback(btScalar timeStep) {
  /*
    for(int i=0;i<num_agents;i++){
    agent[i]->body->clearForces();
    agent[i]->left_wheel->clearForces();
    agent[i]->right_wheel->clearForces();
    agent[i]->body->applyGravity();
    agent[i]->right_wheel->applyGravity();
    agent[i]->left_wheel->applyGravity();
    agent[i]->right_hinge->setLimit(0.0, 0.0);
    agent[i]->right_hinge->enableAngularMotor(false, 0,0);
    agent[i]->left_hinge->setLimit(0.0, 0.0);
    agent[i]->left_hinge->enableAngularMotor(false, 0,0);
    agent[i]->body->setActivationState(0);
    agent[i]->right_wheel->setActivationState(0);
    agent[i]->left_wheel->setActivationState(0);
    agent[i]->right_hinge->setMotorTarget(0.0, timeStep);
    agent[i]->left_hinge->setMotorTarget(0.0,timeStep);
    }
*/
}

/* ---------------------------------------- */
/*
void myTickCallback(btDynamicsWorld *world, btScalar timeStep) {
    Parameters *w = static_cast<Parameters *>(world->getWorldUserInfo());
    w->myProcessCallback(timeStep);
}
*/

/* ---------------------------------------- */

// This function instialized the physics world and should not be touched
void Parameters:: init_physics_param( void ){

      this->collisionConfig=new btDefaultCollisionConfiguration();
      this->dispatcher=new btCollisionDispatcher(collisionConfig);
      btVector3 worldMin(-5,-1,-5);
      btVector3 worldMax(5,1,5);
      this->broadphase = new btAxisSweep3(worldMin,worldMax);
      //btDantzigSolver* mlcp = new btDantzigSolver();
      //btMLCPSolver* sol = new btMLCPSolver(mlcp);
      this->solver=new btSequentialImpulseConstraintSolver();
      this->world=new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfig);
      this->world->setGravity(btVector3(0.0,-9.81,0.0));	//gravity on Earth
      //this->world->setInternalTickCallback(myTickCallback,static_cast<void *>(this),false);
      this->world->getSolverInfo().m_numIterations = 110;
      //world->getSolverInfo().m_damping = 0.8f; // bullet default 1.0
      //world->getSolverInfo().m_tau = 0.6f; // bullet default 0.6
      world->getSolverInfo().m_solverMode |= SOLVER_USE_2_FRICTION_DIRECTIONS  | SOLVER_SIMD;// | SOLVER_RANDMIZE_ORDER;
      gContactAddedCallback = callbackFunc;
  }

/* ---------------------------------------------------------------------------------------- */



/* ---------------------------------------------------------------------------------------- */

Parameters::~Parameters(){

//  //Delete run_name array
//  delete[] run_name;
  
//  //Delete network objects
//  for(int r=0; r < num_agents; r++)
//    delete nets[r];
//  delete[] nets;
  
//  //Delete ga object
//  delete ga;
  
//  //Delete agents objects
// /* for (int i = 0; i < num_agents; i++){
//    delete agent[i];
//  }
//  delete[] agent;*/
  
//  //Delete object_data
// /* for(int b=0; b < num_objects; b++)
//    delete[] objects_data[b];
//  delete[] objects_data; */
//  delete collision_peer;

//   //Delete agents objects
//  for (int i = 0; i < num_agents; i++){
//      world->removeCollisionObject(agent[i]->body);
//      btMotionState* motionState=agent[i]->body->getMotionState();
//      btCollisionShape* shape=agent[i]->body->getCollisionShape();
//      delete shape;
//      delete motionState;
//      delete agent[i]->body;

//      world->removeCollisionObject(agent[i]->right_wheel);
//      motionState=agent[i]->right_wheel->getMotionState();
//      shape=agent[i]->right_wheel->getCollisionShape();
//      delete shape;
//      delete motionState;
//      delete agent[i]->right_wheel;

//      world->removeCollisionObject(agent[i]->left_wheel);
//      motionState=agent[i]->left_wheel->getMotionState();
//      shape=agent[i]->left_wheel->getCollisionShape();
//      delete shape;
//      delete motionState;
//      delete agent[i]->left_wheel;

//      for (i=world->getNumConstraints()-1; i>=0 ;i--)
//      {
//          btTypedConstraint* constraint = world->getConstraint(i);
//          world->removeConstraint(constraint);
//          delete constraint;
//      }

//      delete agent[i];
//  }
//  delete[] agent;

//  //Delete object object
//  for (int i = 0; i < num_objects; i++){
//      world->removeCollisionObject(object[i]->body);
//      btMotionState* motionState=object[i]->body->getMotionState();
//      btCollisionShape* shape=object[i]->body->getCollisionShape();
//      delete object[i]->body;
//      delete object[i];
//      delete shape;
//      delete motionState;
//  }
//  delete plane;
//  delete[] object;
//  delete dispatcher;
//  delete collisionConfig;
//  delete solver;
//  delete broadphase;
//  delete world;
//  GSL_randon_generator::free_generator( );
  
}



/* ------------------------------------------------------------------------ */

// This function reset the physics state every evaluation and sj=hould not be touched

void	Parameters::resetPhysicsState()
{


    int numObjects = 0;
    int i;

    if (world)
    {
        int numConstraints = world->getNumConstraints();
        for (i=0;i<numConstraints;i++)
        {
            world->getConstraint(0)->setEnabled(true);
        }
        numObjects = world->getNumCollisionObjects();

        ///create a copy of the array, not a reference!
        btCollisionObjectArray copyArray = world->getCollisionObjectArray();




        for (i=0;i<numObjects;i++)
        {
            btCollisionObject* colObj = copyArray[i];
            btRigidBody* body = btRigidBody::upcast(colObj);
            if (body)
            {
                if (body->getMotionState())
                {
                    btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
                    myMotionState->m_graphicsWorldTrans = myMotionState->m_startWorldTrans;
                    body->setCenterOfMassTransform( myMotionState->m_graphicsWorldTrans );
                    colObj->setInterpolationWorldTransform( myMotionState->m_startWorldTrans );
                    colObj->forceActivationState(ACTIVE_TAG);
                    colObj->activate();
                    colObj->setDeactivationTime(0);
                    //colObj->setActivationState(WANTS_DEACTIVATION);
                }
                //removed cached contact points (this is not necessary if all objects have been removed from the dynamics world)
                if (world->getBroadphase()->getOverlappingPairCache())
                    world->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(colObj->getBroadphaseHandle(),dispatcher);

                btRigidBody* body = btRigidBody::upcast(colObj);
                if (body && !body->isStaticObject())
                {
                    btRigidBody::upcast(colObj)->setLinearVelocity(btVector3(0,0,0));
                    btRigidBody::upcast(colObj)->setAngularVelocity(btVector3(0,0,0));
                }
            }

        }

        ///reset some internal cached data in the broadphase
        world->getBroadphase()->resetPool(dispatcher);
        world->getConstraintSolver()->reset();

    }

}


/* ------------------------------------------------------------------------ */



// This is a call back function for detect the collision of objects and robots in physics world

bool callbackFunc(btManifoldPoint& cp, const btCollisionObjectWrapper* ob1, int id1, int index1, const btCollisionObjectWrapper* ob2, int id2, int index2)
{

    bool exist = false;

     const btCollisionObject* collision_object1 = ob1->getCollisionObject();
     const btCollisionObject* collision_object2 = ob2->getCollisionObject();

     World_Entity* object1 = (World_Entity*)collision_object1->getUserPointer();
     World_Entity* object2 = (World_Entity*)collision_object2->getUserPointer();

     // if ((!object1->is_crashed()) || (!object2->is_crashed())){
//                object1->set_crashed( true );
//                object2->set_crashed( true );
                if(object2->get_type_id()== ROBOT){
                    if(object1->get_type_id() != 20){
                    //printf("\n Robot colliding with object = %d",object1->get_type_id());
                    object1->set_crashed( true );
                    object2->set_crashed( true );
                    }
                }



    return false;
}


/* ---------------------------------------- */

void Parameters::init_random_generator( void ){
  GSL_randon_generator::init_generator( root_seed );
}

/* ---------------------------------------- */

void Parameters::init_objects( ){
  plane = new SIMPLE_Plane( world );
  plane->body->setUserPointer(plane);

  if( num_objects ){


    object = new SIMPLE_Objects*[num_objects];
    
    int count = 0, ob = 0;

    while( count < num_bricks && ob < num_objects ) {

      object[ob] = new SIMPLE_Brick( ob, objects_data[ob], world);
      object[ob]->body->setUserPointer(object[ob]);
      object[ob]->set_crashed(false);
      ob++;
      count++;
    }
    count = 0;
    while( count < num_cylinders && ob < num_objects ) {
      object[ob] = new SIMPLE_Cylinder ( ob, objects_data[ob], world);
      object[ob]->body->setUserPointer(object[ob]);
      ob++;
      count++;
    }
    count = 0;
    while( count < num_spheres && ob < num_objects ) {
      object[ob] = new SIMPLE_Sphere ( ob, objects_data[ob], world);
      object[ob]->body->setUserPointer(object[ob]);
      ob++;
      count++;
    }
    count = 0;
    while( count < num_lights && ob < num_objects ) {
      object[ob] = new SIMPLE_Light ( objects_data[ob], world );
      ob++;
      count++;
    }
  }
}

/* ---------------------------------------- */

void Parameters::init_agents( void ){
  vector <double> colour;
  colour.assign(3, 1.0);
  agent = new SIMPLE_Agents*[num_agents];
  for (int r = 0; r < num_agents; r++){
    agent[r] = new SIMPLE_Agents( r, simulation_time_step, world);
    agent[r]->set_crashed(false);
    agent[r]->body->setUserPointer(agent[r]);
    agent[r]->left_wheel->setUserPointer(agent[r]);
    agent[r]->right_wheel->setUserPointer(agent[r]);
    colour[0] = 0.5;
    colour[1] = 0.5;
    colour[2] = 1.0;
    agent[r]->set_colour( colour );
  }
  
}


/* ---------------------------------------- */

void Parameters::init_controllers( void ){


  if ( !strcmp ( type_of_controller, "perceptron" ) ){
    for(int r=0; r < num_agents; r++) {
        // here you need to create agent's controller (i.e. neural networks) as a number of agents you have
        //   in the world (usually single agent)
    }
  }
  else{
    cerr << " in Parameters::init_controllers - Controller type not found " << endl;
    exit(0);
  }
}

/* ---------------------------------------- */

void Parameters::init_ga( const int num_allele_per_solution, const int num_bases_per_allele ){  

  if ( !strcmp ( type_of_ga, "roulette_wheel" ) ){ printf("create ga...\n");
    ga = new Roulette_wheel ( num_allele_per_solution, num_bases_per_allele, num_agents );
    /*   
	 char file_dir[500];
	 sprintf(file_dir, "/home/elt7/Simulations/task_switch/EXP/GENOME/"); 
	 char file_name[500];
	 sprintf(file_name, "H"); 
	 ga->upload_genome_from_file( file_dir, file_name, 0 );
    */

  }
  else{
    cerr << " in Parameters::init_controllers - GA type not found " << endl;
    exit(0);
  }
}

/* ---------------------------------------- */

void Parameters::dump_simulation_seed( void ){
  char fileName[500];
  sprintf(fileName, "%s%s.seed", rootSeedFileDir, rootSeedFileName);
  ofstream outfile ( fileName, ios::app);
  outfile.setf(ios::fixed);
  outfile << "ROOT_SEED=" << root_seed << endl; 
  outfile.close();
}

/* ---------------------------------------- */

void Parameters::reset_seed( void ){
  GSL_randon_generator::reset_seed( root_seed );
}

/* ---------------------------------------- */
