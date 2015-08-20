#include "simple_agents.h"
/* ----------------------------------------------------------------------- */
/*                              CONSTRUCTOR                                */
/* ----------------------------------------------------------------------- */
SIMPLE_Agents::SIMPLE_Agents(int ind, double _simulation_time_step,  btDynamicsWorld* _world) : World_Entity ( ) {
  pos.assign(3, 0.0);
  dim.assign(3, 0.0);
  rot.assign(3, 0.0);
  vel.assign(2, 0.0);
  from1.assign(num_IR_sensors,btVector3(0.0,0.0,0.0));
  from2 = btVector3(0.0,0.0,0.0);
  to1.assign(num_IR_sensors,btVector3(0.0,0.0,0.0));
  to2.assign(num_camera_sectors*num_camera_rays_per_sectors,btVector3(0.0,0.0,0.0));
  colour.assign(3, 0.0);
  simulation_time_step   = _simulation_time_step;
  frozen = false;

  dim[0] = robot_radius;
  dim[1] = robot_height;
  dim[2] = 0.0;

  index = ind;
  type_id = ROBOT;
  collision_counter = 0;

  world = _world;
  addRobotPhysics(pos,rot);

    map = new Occupancy_Map();
}

/* ----------------------------------------------------------------------- */
/*                             END CONSTRUCTOR                             */
/* ----------------------------------------------------------------------- */
SIMPLE_Agents::~SIMPLE_Agents( ){ }

/* ----------------------------------------------------------------------- */
/*                             SET AGENT POSITION                          */
/* ----------------------------------------------------------------------- */

void SIMPLE_Agents::set_pos( const vector <double> &_pos ){}

/* ----------------------------------------------------------------------- */
/*                             GET AGENT POSITION                          */
/* ----------------------------------------------------------------------- */

const vector <double> SIMPLE_Agents::get_pos( ){
    pos[0] = body->getWorldTransform().getOrigin().x();
    pos[1] = body->getWorldTransform().getOrigin().y();
    pos[2] = body->getWorldTransform().getOrigin().z();
    return pos;
}


/* ----------------------------------------------------------------------- */
/*                             Create ROBOT PHYSICS                        */
/* ----------------------------------------------------------------------- */
void SIMPLE_Agents::addRobotPhysics( const vector <double> &_pos, const vector <double> &_rot ){
     mass = 0.150;
     btQuaternion rotation;
     rotation.setEulerZYX( 0.0, _rot[1], 0.0);
     btVector3 position = btVector3(_pos[0],robot_height * 0.65,_pos[2]);
     btMotionState* motion = new btDefaultMotionState(btTransform(rotation, position));
     btCylinderShape* cylinder1=new btCylinderShape(btVector3(robot_radius,robot_height*0.5,robot_radius));
     cylinder1->setMargin(robot_radius*0.5);
     btVector3 inertia(0.0,0.0,0.0);
     cylinder1->calculateLocalInertia(mass,inertia);
     btRigidBody::btRigidBodyConstructionInfo info(mass,motion,cylinder1,inertia);
     //info.m_restitution = 0.0f;
     info.m_friction = 0.5f;
     //info.m_rollingFriction = 0.1;
     body=new btRigidBody(info);
     body->setLinearFactor(btVector3(1,1,1));
     body->setAngularFactor(btVector3(0,1,0));
     body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK );
     world->addRigidBody(body);



       btCylinderShape* wheel1 = new btCylinderShape(btVector3(wheel_radius,wheel_width,wheel_radius));
       double mass1 = 0.15;
       btVector3 inertia1(0.0,0.0,0.0);
       wheel1->calculateLocalInertia(mass1,inertia1);
       btTransform t1;
       t1.setIdentity();
       t1.setOrigin(btVector3(_pos[0], wheel_radius + pos[1], half_wheel_distance + pos[2]));
       btQuaternion rotation1;
       rotation1.setEulerZYX( 0.0, _rot[1], 1.57072428);
       t1.setRotation(rotation1);
       btMotionState* motion1=new btDefaultMotionState(t1);
       btRigidBody::btRigidBodyConstructionInfo info1(mass1,motion1,wheel1,inertia1);
       //info1.m_restitution = 0.0f;
       info1.m_friction = 0.45f;
       //info1.m_rollingFriction = 10.0;
       right_wheel=new btRigidBody(info1);
       world->addRigidBody(right_wheel);
       right_wheel->setLinearFactor(btVector3(1,1,1));
       right_wheel->setAngularFactor(btVector3(1,1,1));
       btVector3 axisA(0.f, 0.f, 1.0f);
       btVector3 axisB(0.f, 1.f, 0.f);
       btVector3 pivotA(0.f, -robot_height* 0.22, half_wheel_distance);
       btVector3 pivotB(0.0f, 0.0f, 0.0f);
       right_hinge = new btHingeConstraint(*body, *right_wheel, pivotA, pivotB, axisA, axisB);
       btJointFeedback* feedback1 = new btJointFeedback;
       feedback1->m_appliedForceBodyA = btVector3(0, 0, 0);
       feedback1->m_appliedForceBodyB = btVector3(0, 0, 0);
       feedback1->m_appliedTorqueBodyA = btVector3(0, 0, 0);
       feedback1->m_appliedTorqueBodyB = btVector3(0, 0, 0);
       right_hinge->setJointFeedback(feedback1);
       right_hinge->enableFeedback(true);
       right_hinge->setLimit(1,-1,1.0,0.3,1);
       right_hinge->enableAngularMotor(false, 0,0);
       //right_hinge->setOverrideNumSolverIterations(1000);
       world->addConstraint(right_hinge, true);



       btCylinderShape* wheel2 = new btCylinderShape(btVector3(wheel_radius,wheel_width,wheel_radius));
       double mass2 = 0.15;
       btVector3 inertia2(0.0,0.0,0.0);
       wheel2->calculateLocalInertia(mass2,inertia2);
       btTransform t2;
       t2.setIdentity();
       t2.setOrigin(btVector3(pos[0], wheel_radius + pos[1], half_wheel_distance + pos[2]));
       btQuaternion rotation2;
       rotation2.setEulerZYX( 0.0, _rot[1], 1.57072428);
       t2.setRotation(rotation2);
       btMotionState* motion2=new btDefaultMotionState(t2);
       //wheel2->setMargin(0.0);
       btRigidBody::btRigidBodyConstructionInfo info2(mass2,motion2,wheel2,inertia2);
       //info2.m_restitution = 0.0f;
       info2.m_friction = 0.45f;
      // info2.m_rollingFriction = 10.0;
       left_wheel=new btRigidBody(info2);
       right_wheel->setLinearFactor(btVector3(1,1,1));
       right_wheel->setAngularFactor(btVector3(1,1,1));
       world->addRigidBody(left_wheel);
       btVector3 axisA1(0.0f, 0.0f, 1.0f);
       btVector3 axisB1(0.0f, 1.0f, 0.0f);
       btVector3 pivotA1(0.f, -robot_height* 0.22,-half_wheel_distance);
       btVector3 pivotB1(0.0f, 0.0f, 0.0f);
       left_hinge = new btHingeConstraint(*body, *left_wheel, pivotA1, pivotB1, axisA1, axisB1);
       left_hinge->enableFeedback(true);
       btJointFeedback* feedback = new btJointFeedback;
       feedback->m_appliedForceBodyA = btVector3(0, 0, 0);
       feedback->m_appliedForceBodyB = btVector3(0, 0, 0);
       feedback->m_appliedTorqueBodyA = btVector3(0, 0, 0);
       feedback->m_appliedTorqueBodyB = btVector3(0, 0, 0);
       left_hinge->setJointFeedback(feedback);
       left_hinge->setLimit(1,-1,1.0,0.3,1);
       left_hinge->enableAngularMotor(false, 0,0);
       //left_hinge->setOverrideNumSolverIterations(1000);
       world->addConstraint(left_hinge, true);

}
/* ----------------------------------------------------------------------- */
/*                             SET AGENT ROTATION                          */
/* ----------------------------------------------------------------------- */

void SIMPLE_Agents::set_robot_pos_rot( const vector <double> &_pos, const vector <double> &_rot){

    btTransform trans;
    trans.setIdentity();
    btQuaternion rotation;
    rotation.setEulerZYX( 0.0, _rot[1], 0.0);
    //btTransform trans = body->getWorldTransform();
    trans.setOrigin(btVector3(_pos[0],robot_height * 0.65,_pos[2]));
    trans.setRotation(rotation);
    btMotionState* motion=new btDefaultMotionState(trans);
    body->setMotionState(motion);
    btTransform trans1;
    trans1.setIdentity();
    btQuaternion rotation1;
    rotation1.setEulerZYX( 0.0, _rot[1], 1.57072428);
    //btTransform trans1 = right_wheel->getWorldTransform();
    trans1.setOrigin(btVector3((_pos[0] + (half_wheel_distance * sin(_rot[1]))), wheel_radius, (_pos[2] + (half_wheel_distance * cos(_rot[1])))));
    trans1.setRotation(rotation1);
    btMotionState* motion1=new btDefaultMotionState(trans1);
    right_wheel->setMotionState(motion1);
    btTransform trans2;
    trans2.setIdentity();
    btQuaternion rotation2;
    rotation2.setEulerZYX( 0.0,_rot[1], 1.57072428);
    //btTransform trans2 = left_wheel->getWorldTransform();
    trans2.setOrigin(btVector3((_pos[0] + (-half_wheel_distance * sin(_rot[1]))), wheel_radius,(_pos[2] + (-half_wheel_distance * cos(_rot[1])))));
    trans2.setRotation(rotation2);
    btMotionState* motion2=new btDefaultMotionState(trans2);
    left_wheel->setMotionState(motion2);

}

/* ----------------------------------------------------------------------- */
/*                             GET AGENT ROTATION                          */
/* ----------------------------------------------------------------------- */
 void SIMPLE_Agents::set_rot(const vector <double> &_rot){

 }


/* ----------------------------------------------------------------------- */
/*                             GET AGENT ROTATION                          */
/* ----------------------------------------------------------------------- */


const vector <double> SIMPLE_Agents::get_rot(){
    btMatrix3x3 rotMatrix = btMatrix3x3(body->getWorldTransform().getRotation());
    float rotX,rotY,rotZ;
    rotMatrix.getEulerZYX(rotZ,rotY,rotX);
    rot[0] = rotX;
    rot[1] = rotY;
    rot[2] = rotZ;
    return rot;
}




/* ----------------------------------------------------------------------- */
/*                            SET AGENT VELOCITY                           */
/* ----------------------------------------------------------------------- */
void SIMPLE_Agents::set_vel( const vector <double> &outputs ){
  right_out = (outputs[0]-outputs[1]);
  left_out  = (outputs[2]-outputs[3]);
  //printf("\n right out = %f left out = %f",right_out,left_out);
  
 vel[0] = (right_out*max_vel);// + (((2.0*gsl_rng_uniform_pos (GSL_randon_generator::r_rand)*vel_noise) - vel_noise)  * NOISE_LEVEL );
  if( vel[0] > max_vel ) vel[0] = max_vel;
  else if( vel[0] < min_vel ) vel[0] = min_vel;
  
  vel[1] = (left_out*max_vel);// + (((2.0*gsl_rng_uniform_pos (GSL_randon_generator::r_rand)*vel_noise) - vel_noise)  * NOISE_LEVEL );
  if( vel[1] > max_vel ) vel[1] = max_vel;
  else if( vel[1] < min_vel ) vel[1] = min_vel;

}

/* ------------------------------------------------------------------------------------- */
/* DIFFERENTIAL DRIVE ROBOTS - DUDEK, JENKIN Computational Principles of Mobile Robotics */
/*                               UPDATE POSITION ROTATION                                */
/* ------------------------------------------------------------------------------------- */
void SIMPLE_Agents::update_pos_rot( void ){
    //body->setActivationState(1);
    //right_wheel->setActivationState(1);
    //right_hinge->setLimit(1,-1,0,0.3,1);
    right_hinge->enableAngularMotor(true,vel[0],0.0004);
    //left_wheel->setActivationState(1);
    //left_hinge->setLimit(1,-1,0,0.3,1);
    left_hinge->enableAngularMotor(true,vel[1],0.0004);


}

/* ----------------------------------------------------------------------- */
/*                              IR_READING                          */
/* ----------------------------------------------------------------------- */

void SIMPLE_Agents::get_IR_reading( vector <double> &_reading){
    double x,z,X,Z,rotation = 0.0;
    this->pos = this->get_pos();
    double y = pos[1] + 0.011;
    btMatrix3x3 m = btMatrix3x3(body->getWorldTransform().getRotation());
    double rfPAngle = btAsin(-m[1][2]);
       if ( rfPAngle < SIMD_HALF_PI )   {
           if ( rfPAngle > -SIMD_HALF_PI ) rotation = btAtan2(m[0][2],m[2][2]);
            else rotation = -btAtan2(-m[0][1],m[0][0]);
        }
        else rotation = btAtan2(-m[0][1],m[0][0]);

 //   IR0 reading
      x = pos[0]+((robot_radius) * cos(-rotation + 0.3));   //calc IR sensor X based on robot radius
      z = pos[2]+((robot_radius) * sin(-rotation + 0.3));
      from1[0] = btVector3(x, y, z);                        //sensor position based on robot rotation vector to hold btVector
      X = x+((IR_range) * cos(-rotation + 0.3 ));           //calc IR ray end position based on IR range and placement of sensor on the robot
      Z = z+((IR_range) * sin(-rotation + 0.3 ));
      to1[0] = btVector3( X, y, Z );                        //Max reading pos of the IR based on rotation and IR range
      btCollisionWorld::ClosestRayResultCallback res0(from1[0], to1[0]); //struct for the closest ray callback
      //uncomment below line if you experience any ray pentration problem to the object this might happened with very slow machine
      //res0.m_flags = 0xFFFFFFFF;
       this->world->rayTest(from1[0], to1[0], res0);        //check for ray collision between the coords sets
    if(res0.hasHit()){
        _reading[0] = IR_range*res0.m_closestHitFraction ;
        to1[0]=res0.m_hitPointWorld;                        //update the vector with the btVector results -> used to render it in openGL
        take_occupancy_reading(to1[0].getX(), to1[0].getZ());
    }

 //   IR7 reading
      x = pos[0]+((robot_radius) * cos(-rotation - 0.3));
      z = pos[2]+((robot_radius) * sin(-rotation - 0.3));
      from1[1] = btVector3(x, y, z);
      X = x+((IR_range) * cos(-rotation - 0.3 ));
      Z = z+((IR_range) * sin(-rotation - 0.3 ));
      to1[1] = btVector3( X, y, Z );
      btCollisionWorld::ClosestRayResultCallback res7(from1[1], to1[1]);
      //uncomment below line if you experience any ray pentration problem to the object this might happened with very slow machine
      //res7.m_flags = 0xFFFFFFFF;
      this->world->rayTest(from1[1], to1[1], res7);
      if(res7.hasHit()){
          _reading[1] = IR_range*res7.m_closestHitFraction;
          to1[1]=res7.m_hitPointWorld;
          take_occupancy_reading(to1[1].getX(), to1[1].getZ());
      }

 //   IR1 reading corrospond to epuck
      x = pos[0]+((robot_radius) * cos(-rotation + 0.8));
      z = pos[2]+((robot_radius) * sin(-rotation + 0.8));
      from1[2] = btVector3(x, y, z);
      X = x+((IR_range) * cos(-rotation + 0.8 ));
      Z = z+((IR_range) * sin(-rotation + 0.8 ));
      to1[2] = btVector3( X, y, Z );
      btCollisionWorld::ClosestRayResultCallback res1(from1[2], to1[2]);
      //uncomment below line if you experience any ray pentration problem to the object this might happened with very slow machine
      //res1.m_flags = 0xFFFFFFFF;
      this->world->rayTest(from1[2], to1[2], res1);
      if(res1.hasHit()){
        _reading[2] = IR_range*res1.m_closestHitFraction;
        to1[2]=res1.m_hitPointWorld;
          take_occupancy_reading(to1[2].getX(), to1[2].getZ());
      }


 //   IR6 reading
      x = pos[0]+((robot_radius) * cos(-rotation - 0.8));
      z = pos[2]+((robot_radius) * sin(-rotation - 0.8));
      from1[3] = btVector3(x, y, z);
      X = x+((IR_range) * cos(-rotation - 0.8 ));
      Z = z+((IR_range) * sin(-rotation - 0.8 ));
      to1[3] = btVector3( X, y, Z );
      btCollisionWorld::ClosestRayResultCallback res6(from1[3], to1[3]);
      //uncomment below line if you experience any ray pentration problem to the object this might happened with very slow machine
      //res6.m_flags = 0xFFFFFFFF;
      this->world->rayTest(from1[3], to1[3], res6);
      if(res6.hasHit()){
          _reading[3] = IR_range*res6.m_closestHitFraction;
          to1[3]=res6.m_hitPointWorld;
          take_occupancy_reading(to1[3].getX(), to1[3].getZ());
     }

 //   IR2 reading
      x = pos[0]+((0.028) * cos(-rotation + 1.57));
      z = pos[2]+((0.028) * sin(-rotation + 1.57));
      from1[4] = btVector3(x, y, z);
      X = x+((0.049) * cos(-rotation + 1.57 ));
      Z = z+((0.049) * sin(-rotation + 1.57));
      to1[4] = btVector3( X, y, Z );
      btCollisionWorld::ClosestRayResultCallback res2(from1[4], to1[4]);
      //uncomment below line if you experience any ray pentration problem to the object this might happened with very slow machine
      //res2.m_flags = 0xFFFFFFFF;
      this->world->rayTest(from1[4], to1[4], res2);
      if(res2.hasHit()){
          _reading[4] = 0.049*(res2.m_closestHitFraction) - 0.009;
          to1[4]=res2.m_hitPointWorld;
          take_occupancy_reading(to1[4].getX(), to1[4].getZ());
      }

 //   IR5 reading
      x = pos[0]+((0.028) * cos(-rotation - 1.57));
      z = pos[2]+((0.028) * sin(-rotation - 1.57));
      from1[5] = btVector3(x, y, z);
      X = x+((0.049) * cos(-rotation - 1.57 ));
      Z = z+((0.049) * sin(-rotation - 1.57 ));
      to1[5] = btVector3( X, y, Z );
      btCollisionWorld::ClosestRayResultCallback res5(from1[5], to1[5]);
      //uncomment below line if you experience any ray pentration problem to the object this might happened with very slow machine
      //res5.m_flags = 0xFFFFFFFF;
      this->world->rayTest(from1[5], to1[5], res5);
      if(res5.hasHit()){
           _reading[5] = 0.049*res5.m_closestHitFraction - 0.009;
           to1[5]=res5.m_hitPointWorld;
          take_occupancy_reading(to1[5].getX(), to1[5].getZ());
      }


 //   IR3 reading
      x = pos[0]+((robot_radius) * cos(-rotation + 2.64));
      z = pos[2]+((robot_radius) * sin(-rotation + 2.64));
      from1[6] = btVector3(x, y, z);
      X = x+((IR_range) * cos(-rotation + 2.64 ));
      Z = z+((IR_range) * sin(-rotation + 2.64 ));
      to1[6] = btVector3( X, y, Z );
      btCollisionWorld::ClosestRayResultCallback res3(from1[6], to1[6]);
      //uncomment below line if you experience any ray pentration problem to the object this might happened with very slow machine
      //res3.m_flags = 0xFFFFFFFF;
      this->world->rayTest(from1[6], to1[6], res3);
      if(res3.hasHit()){
          _reading[6] = IR_range*res3.m_closestHitFraction;
          to1[6]=res3.m_hitPointWorld;
          take_occupancy_reading(to1[6].getX(), to1[6].getZ());
      }

  //   IR4 reading
      x = pos[0]+((robot_radius) * cos(-rotation - 2.64));
      z = pos[2]+((robot_radius) * sin(-rotation - 2.64));
      from1[7] = btVector3(x, y, z);
      X = x+((IR_range) * cos(-rotation - 2.64 ));
      Z = z+((IR_range) * sin(-rotation - 2.64 ));
      to1[7] = btVector3( X, y, Z );
      btCollisionWorld::ClosestRayResultCallback res4(from1[7], to1[7]);
      //uncomment below line if you experience any ray pentration problem to the object this might happened with very slow machine
      //res4.m_flags = 0xFFFFFFFF;
      this->world->rayTest(from1[7], to1[7], res4);
      if(res4.hasHit()){
          _reading[7] = IR_range*res4.m_closestHitFraction;
          to1[7]=res4.m_hitPointWorld;
          take_occupancy_reading(to1[7].getX(), to1[7].getZ());
      }

//        for( int i = 0; i < num_IR_sensors; i++){
//            printf(" \nIR%d distance reading= %f ",i,_reading[i]);
//        }

    //calibrating distance to IR value reading according to a line equations
  for(int i = 0; i < num_IR_sensors; i++){
       if (_reading[i] > 0.03 && _reading[i] <= 0.04)
           _reading[i] = -20600 * _reading[i] + 924;
       else if (_reading[i] > 0.02 && _reading[i] <= 0.03)
           _reading[i] = -37000 * _reading[i] + 1416;
       else if (_reading[i] > 0.01 && _reading[i] <= 0.02)
           _reading[i] = -153500 * _reading[i] + 3746;
       else if (_reading[i] > 0.005 && _reading[i] <= 0.01)
           _reading[i] = -252600 * _reading[i] + 4737;
       else if (_reading[i] >= 0.0 && _reading[i] <= 0.005 )
           _reading[i] = -124200 * _reading[i] + 4095;

  }
  for( int i = 0; i < num_IR_sensors; i++){
      printf("\n IR%d distance reading= %f ",i,_reading[i]);
  }

}


/* ---------------------------------------------------------------------------------------- */

void SIMPLE_Agents::add_noise( vector <double> &readings ){
  //add noise

  for(int i = 0; i < num_IR_sensors; i++){
    if( readings[i] == -1.0 ){
        //just background noise
        readings[i] = (((double)(gsl_rng_uniform_int(GSL_randon_generator::r_rand, 100))) * NOISE_LEVEL );
        //printf("\n IR%d with noise reading= %f ",i,readings[i]);
        readings[i] /= (double)(max_ir_reading);

    }
    else{
        double noise = ((rint)(gsl_ran_gaussian(GSL_randon_generator::r_rand, 30)) * NOISE_LEVEL);
        readings[i] += noise ;
        if( readings[i] > max_ir_reading ) readings[i] = max_ir_reading;
        else if ( readings[i] < 0.0 ) readings[i] = 0.0;
        //printf("\n IR%d with noise reading= %f  noise = %f",i,readings[i], noise);
        readings[i] /= (double)(max_ir_reading);
    }
  }


}

/* ----------------------------------------------------------------------- */
/*                              IR_READING                          */
/* ----------------------------------------------------------------------- */

void SIMPLE_Agents::get_camera_reading( vector <double> &_reading){
    double x,z,X,Z,rotation = 0.0;
    int found_red,found_green,rg_counter; //red for robot and green for object
    this->pos = this->get_pos();
    btMatrix3x3 m = btMatrix3x3(body->getWorldTransform().getRotation());
    double rfPAngle = btAsin(-m[1][2]);
    if ( rfPAngle < SIMD_HALF_PI ){
          if ( rfPAngle > -SIMD_HALF_PI ) rotation = btAtan2(m[0][2],m[2][2]);
          else rotation = -btAtan2(-m[0][1],m[0][0]);
        }
    else rotation = btAtan2(-m[0][1],m[0][0]);

    x = pos[0]+(robot_radius * cos(-rotation));
    z = pos[2]+(robot_radius * sin(-rotation));
     btVector3 from(x, pos[1]+0.007, z);
    from2 = from;
    for(int s=0; s < num_camera_sectors*num_camera_rays_per_sectors; s++)
        to2[s] = from;
    for(int s=0; s < num_camera_sectors; s++){
       found_red = found_green =rg_counter = 0;
    for(int r=0; r < num_camera_rays_per_sectors; r++){
    X = x +(camera_ray * cos(-rotation - 0.235619449 + ((r + s*num_camera_rays_per_sectors)*0.0261799388)));
    Z = z +(camera_ray * sin(-rotation - 0.235619449 + ((r + s*num_camera_rays_per_sectors)*0.0261799388)));
    btVector3 to( X, pos[1], Z );
    btCollisionWorld::ClosestRayResultCallback res(from, to);
    //uncomment below line if you experience any ray pentration problem to the object this might happened with very slow machine
    //res.m_flags = 0xFFFFFFFF;
    this->world->rayTest(from, to, res);
    to2[r + s*num_camera_rays_per_sectors] = to;

    if(res.hasHit()){
        if(camera_ray*res.m_closestHitFraction < 0.005){
            to2[r + s*num_camera_rays_per_sectors] = res.m_hitPointWorld;
            break;
        }else{
        to2[r + s*num_camera_rays_per_sectors] = res.m_hitPointWorld;

        World_Entity* object1 = (World_Entity*) res.m_collisionObject->getUserPointer();
        if(!found_red){
             if(object1->get_type_id() == ROBOT /*&& object1->get_index() == 0*/)
             {
                  found_red = 1;
                  rg_counter++;
                  if(rg_counter > 1){
                      _reading[num_camera_sectors-s-1] = 1.0; // the robot see red and green
                      break;
                  }
                  else{
                      _reading[num_camera_sectors-s-1] = 0.4; // the robot see only red color
                      continue;
                  }
             }
        }
        if(!found_green){
             if(object1->get_type_id() == 4 /*&& object1->get_index() == 1*/) //object
             {
                  found_green = 1;
                  rg_counter++;
                  if(rg_counter > 1){
                      _reading[num_camera_sectors-s-1] = 1.0; // the robot see both red and green color
                      break;
                  }
                  else{
                      _reading[num_camera_sectors-s-1] = 0.7; // the robot see only green color
                      continue;
                  }
             }
           }
        }
     }
   }
 }

}

/* ----------------------------------------------------------------------- */
/*                                PINK NOISE                               */
/* ----------------------------------------------------------------------- */
double SIMPLE_Agents::pinkq( int index ){
  if(medium_term[index] == 0.0 ) 
    medium_term[index] = gsl_rng_uniform_pos(GSL_randon_generator::r_rand);
  else if( gsl_rng_uniform_pos(GSL_randon_generator::r_rand) < prob_medium_term_change )
    medium_term[index] = gsl_rng_uniform_pos(GSL_randon_generator::r_rand);
  
  if(long_term[index] == 0.0 ) 
    long_term[index] = gsl_rng_uniform_pos(GSL_randon_generator::r_rand);
  else if( gsl_rng_uniform_pos(GSL_randon_generator::r_rand) < prob_long_term_change )
    long_term[index] = gsl_rng_uniform_pos(GSL_randon_generator::r_rand);
  
  return (long_term[index] + medium_term[index] + gsl_rng_uniform_pos(GSL_randon_generator::r_rand) )/3.0;
}

#ifdef _GRAPHICS_
/* ----------------------------------------------------------------------- */
/*                    openGL RENDERING FUNCTIONS                           */
/* ----------------------------------------------------------------------- */
void SIMPLE_Agents::render( void ){


    if(body->getCollisionShape()->getShapeType()!=CYLINDER_SHAPE_PROXYTYPE)
        return;
    GLUquadric* quad = gluNewQuadric();
    glColor3f(colour[0], colour[1], colour[2]);
    btVector3 extent=((btCylinderShape*)body->getCollisionShape())->getHalfExtentsWithMargin();
    btVector3 extent10=((btCylinderShape*)body->getCollisionShape())->getHalfExtentsWithoutMargin();
   // printf("\n With margin x= %f y=%f z=%f",extent.getX(),extent.getY(),extent.getZ());
    //printf("\n Without margin x= %f y=%f z=%f",extent10.getX(),extent10.getY(),extent10.getZ());
    //printf("\n differnece x= %f y=%f z=%f",extent.getX()-extent10.getX(),extent.getY()-extent10.getY(),extent.getZ()-extent10.getZ());
    btTransform t;
    body->getMotionState()->getWorldTransform(t);
    float mat[16];
    t.getOpenGLMatrix(mat);
    glPushMatrix();
    glMultMatrixf(mat);
                   glTranslatef(0,extent.y(),0);
                   glRotatef(90,1,0,0);
                  // glRotatef(((rot[2]*360.0)/6.28), 0, 0, 1);
                   gluCylinder(quad,extent.x(),extent.x(),extent.y()*2.0,robot_slices, 1);
                   gluDisk(quad, 0, extent.x(), robot_slices, 1);
                  // glTranslated(2.0,0.0,0.0);

                   glTranslated(0,0,extent.y()*2.0);
                   gluDisk(quad, 0, extent.x(), robot_slices, 1);
                   vector <double> lengths;
                   lengths.assign(3, 0.0);
                   lengths[0] = 0.1;
                   lengths[1] = 0.05;
                   lengths[2] = 0.1;
                   glColor3f(1.0, 0.0, 0.0);
                   glScalef(lengths[0], lengths[1], lengths[2]);
                   glTranslated(0.25,0.0,-0.45);
                   glutSolidCube(0.1f);


        gluDeleteQuadric(quad);
    glPopMatrix();

    if(right_wheel->getCollisionShape()->getShapeType()!=CYLINDER_SHAPE_PROXYTYPE)
        return;
    GLUquadric* quad1 = gluNewQuadric();
    glColor3f(0.0, 0.0,0.0);
    btVector3 extent1=((btCylinderShape*)right_wheel->getCollisionShape())->getHalfExtentsWithMargin();
    btTransform t1;
    right_wheel->getMotionState()->getWorldTransform(t1);
    float mat1[16];
    t1.getOpenGLMatrix(mat1);
    glPushMatrix();
    glMultMatrixf(mat1);
                   glTranslatef(0,extent1.y(),0);
                   glRotatef(90,1,0,0);
                  // glRotatef(((rot[2]*360.0)/6.28), 0, 0, 1);
                   gluCylinder(quad,extent1.x(),extent1.x(),extent1.y()*2.0,robot_slices, 1);
                   gluDisk(quad, 0, extent1.x(), robot_slices, 1);
                   glTranslated(0,0,extent1.y()*2.0);
                   gluDisk(quad, 0, extent1.x(), robot_slices, 1);
                   vector <double> lengths1;
                   lengths1.assign(3, 0.0);
                   lengths1[0] = 0.05;
                   lengths1[1] = 0.05;
                   lengths1[2] = 0.05;
                   glColor3f(1.0, 1.0, 0.0);
                   glScalef(lengths1[0], lengths1[1], lengths1[2]);
                   glTranslated(0.3,0.0,-0.1);
                   glutSolidSphere(0.07f, 20, 20);
                   glTranslated(-0.6,0.0,0.0);
                   glutSolidSphere(0.07f, 20, 20);

       // gluDeleteQuadric(quad1);
    glPopMatrix();

    if(left_wheel->getCollisionShape()->getShapeType()!=CYLINDER_SHAPE_PROXYTYPE)
        return;
    //GLUquadric* quad2 = gluNewQuadric();
    glColor3f(0.0,0.0,0.0);
    btVector3 extent2=((btCylinderShape*)left_wheel->getCollisionShape())->getHalfExtentsWithMargin();
    btTransform t2;
    left_wheel->getMotionState()->getWorldTransform(t2);
    float mat2[16];
    t2.getOpenGLMatrix(mat2);
    glPushMatrix();
    glMultMatrixf(mat2);
                   glTranslatef(0,extent2.y(),0);
                   glRotatef(90,1,0,0);
                  // glRotatef(((rot[2]*360.0)/6.28), 0, 0, 1);
                   gluCylinder(quad1,extent2.x(),extent2.x(),extent2.y()*2.0,robot_slices, 1);
                   gluDisk(quad1, 0, extent2.x(), robot_slices, 1);
                   glTranslated(0.0,0,extent2.y()*2.0);
                   gluDisk(quad1, 0, extent2.x(), robot_slices, 1);
                   vector <double> lengths2;
                   lengths2.assign(3, 0.0);
                   lengths2[0] = 0.05;
                   lengths2[1] = 0.05;
                   lengths2[2] = 0.05;
                   glColor3f(1.0, 1.0, 0.0);
                   glScalef(lengths2[0], lengths2[1], lengths2[2]);
                   glTranslated(0.3,0.0,0.05);
                   glutSolidSphere(0.07f, 20, 20);
                   glTranslated(-0.6,0.0,0.0);
                   glutSolidSphere(0.07f, 20, 20);


        gluDeleteQuadric(quad1);
    glPopMatrix();

    for(int i=0;i< num_IR_sensors;i++){
        glEnable( GL_LINE_SMOOTH );
        glLineWidth(1.25);
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        //glVertex3f(from1.getX(), from1.getY(), from1.getZ());
        glVertex3f(from1[i].getX(), from1[i].getY(), from1[i].getZ());
        glVertex3f(to1[i].getX(), to1[i].getY(), to1[i].getZ());
        glEnd();
    }

    for(int s=0; s < num_camera_sectors; s++){
        for(int r=0; r < num_camera_rays_per_sectors; r++){
            glEnable( GL_LINE_SMOOTH );
            glLineWidth(1.25);
            glColor3f(1.0, 0.0, 1.0);
            glBegin(GL_LINES);
            glVertex3f(from2.getX(), from2.getY(), from2.getZ());
            glVertex3f(to2[s*num_camera_rays_per_sectors+r].getX(), to2[s*num_camera_rays_per_sectors+r].getY(), to2[s*num_camera_rays_per_sectors+r].getZ());
            glEnd();
       }
   }

}

/* ----------------------------------------------------------------------- */
/*                           RANGE AND BEARING SENSORS READING             */
/* ----------------------------------------------------------------------- */

double SIMPLE_Agents::get_randb_reading( vector <double> _to_robot_pos, vector <double> &_reading){
    double work_range = 0.4;
    randb_from = btVector3(0.0,0.0,0.0);
    randb_to   = btVector3(0.0,0.0,0.0);
    this->pos = this->get_pos();
    // get the distance between your robot and to destination robot "_to_robot_pos"
    double range = sqrt(((_to_robot_pos[0]-pos[0])*(_to_robot_pos[0]-pos[0]) + (_to_robot_pos[2]-pos[2])*(_to_robot_pos[2]-pos[2])));
    if(range < work_range){
        _reading[0] = range;

        // get the robot orienation
        btMatrix3x3 m = btMatrix3x3(body->getWorldTransform().getRotation());
        double rfPAngle = btAsin(-m[1][2]);
        if(rfPAngle < SIMD_HALF_PI){
            if(rfPAngle > -SIMD_HALF_PI) this->rotation = btAtan2(m[0][2],m[2][2]);
            else this->rotation = -btAtan2(-m[0][1],m[0][0]);
        }
        else this->rotation = btAtan2(-m[0][1],m[0][0]);
        // check the collision accross the distance between your robot and destination robot
        randb_from = btVector3(_to_robot_pos[0],_to_robot_pos[1]+0.025,_to_robot_pos[2]);
        randb_to   = btVector3(pos[0], pos[1]+0.025, pos[2]);
        btCollisionWorld::ClosestRayResultCallback res(randb_from, randb_to);
        this->world->rayTest(randb_from, randb_to, res);
        if(res.hasHit()){
            World_Entity* object = (World_Entity*) res.m_collisionObject->getUserPointer();
            if(object->get_type_id() == ROBOT && object->get_index() == this->index){

                double bearing,nest_angle,robot_angle;
                robot_angle =rotation;
                if(robot_angle<0.0)
                    robot_angle = TWO_PI + robot_angle;
                nest_angle = -atan2(_to_robot_pos[2]-pos[2], _to_robot_pos[0]-pos[0]);
                if(nest_angle <0.0)
                    nest_angle = TWO_PI + nest_angle;
                bearing = nest_angle - robot_angle;
                if(bearing < 0.0)
                    bearing = TWO_PI + bearing;
                //if you want to add noise bearing
                bearing += (gsl_rng_uniform_pos( GSL_randon_generator::r_rand )*0.30 - 0.15);
                _reading[1] = bearing;

            }
            randb_to =res.m_hitPointWorld;
        }
    }
}

//Functions added by stefan below this line

void SIMPLE_Agents::take_occupancy_reading(double x_coord, double z_coord){
//    map->update_map("run.txt", x_coord, z_coord);

}

#endif
