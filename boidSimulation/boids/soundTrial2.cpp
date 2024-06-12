// Ryan Millett
// MAT201B-2024

#include "Gamma/Analysis.h"
#include "Gamma/Effects.h"
#include "Gamma/Envelope.h"
#include "Gamma/Gamma.h"
#include "Gamma/Oscillator.h"
#include "Gamma/Types.h"
#include "al/app/al_App.hpp"
#include "al/app/al_DistributedApp.hpp"
#include "al/app/al_GUIDomain.hpp"
#include "al/math/al_Random.hpp"
#include "al/math/al_Vec.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/math/al_Functions.hpp"
#include "al/io/al_Imgui.hpp"
#include "al/scene/al_DynamicScene.hpp"
#include "al/scene/al_SynthSequencer.hpp"
#include "al/sound/al_Ambisonics.hpp"
#include "al/sound/al_Dbap.hpp"
#include "al/sound/al_StereoPanner.hpp"
#include "al/sound/al_Vbap.hpp"
#include "al/sound/al_Lbap.hpp"
#include "al/ui/al_Parameter.hpp"
#include "al/ui/al_PresetSequencer.hpp"
#include "al_ext/statedistribution/al_CuttleboneDomain.hpp"
#include "al_ext/statedistribution/al_CuttleboneStateSimulationDomain.hpp"
#include "al/sphere/al_AlloSphereSpeakerLayout.hpp"

#include "../utils/octree.cpp"
#include <iostream>
#include <string>
// #include "classes/boid_4.cpp"

const int CUBE_SIZE = 5;
const int N_CENTERS = 2; //realistically this is 8^N number of centers
const float N_CENTER_RAD = 20.0 / pow(2.0, N_CENTERS);

const int MAX_BOIDS = 10;
const int NEIGHBOR_LIMIT = 100;
// const float MAX_BOID_RADIUS = CUBE_SIZE * 0.1;

const int N_PARTICLES = 1500;

gam::ArrayPow2<float>
tbSin(2048), tbSqr(2048), tbPls(2048), tbDin(2048);

#define SpatializerType StereoPanner
using namespace al;
typedef vector<double> Point;

float rand_float(float orig_val, float delta){
  float HI = orig_val + delta;
  float LO = orig_val - delta;
  return (LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO))));
}

double rand_double(double orig_val, double delta){
  double HI = orig_val + delta;
  double LO = orig_val - delta;
  return (LO + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(HI-LO))));
}

double r() { return rnd::uniformS(); }
Vec3f randomVec3f(float scale = 1.0) {
  return Vec3f(r(), r(), r()) * scale;
}

std::vector<Point> generateCenters(int n, double L) {
  std::vector<Point> centers;
  
  // Start with the initial center of the original cube
  Point p;
  p.push_back(0);
  p.push_back(0);
  p.push_back(0);
  centers.push_back(p);
  
  // Iterate n times to divide the cube
  for (int i = 0; i < n; ++i) {
    std::vector<Point> newCenters;
    double offset = L / (1 << i); // 2^i
    for (const auto& center : centers) {
      // Generate 8 new centers for each current center
      for (int dx = -1; dx <= 1; dx += 2) {
        for (int dy = -1; dy <= 1; dy += 2) {
          for (int dz = -1; dz <= 1; dz += 2) {
            Point tempP;
            tempP.push_back(center[0] + dx * offset / 2);
            tempP.push_back(center[1] + dy * offset / 2);
            tempP.push_back(center[2] + dz * offset / 2);
            newCenters.push_back(tempP);
          }
        }
      }
    }
    centers = newCenters;
  }
    
  return centers;
}

struct Axes {
  void draw(Graphics &g) {
    Mesh mesh(Mesh::LINES);
    // x axis
    mesh.vertex(-CUBE_SIZE, 0, 0);
    mesh.color(1, 0, 0);
    mesh.vertex(CUBE_SIZE, 0, 0);
    mesh.color(1, 0, 0);

    // y axis
    mesh.vertex(0, -CUBE_SIZE, 0);
    mesh.color(0, 1, 0);
    mesh.vertex(0, CUBE_SIZE, 0);
    mesh.color(0, 1, 0);

    // z axis
    mesh.vertex(0, 0, -CUBE_SIZE);
    mesh.color(0, 0, 1);
    mesh.vertex(0, 0, CUBE_SIZE);
    mesh.color(0, 0, 1);

    g.draw(mesh);
  }
};

string slurp(string fileName);  // forward declaration

struct CommonState {
  // particles
  float pointSize;
  // boids
  Pose boid[MAX_BOIDS];

  int i_boids[MAX_BOIDS][NEIGHBOR_LIMIT];
  // XXX - boid vertex colors???
  Pose pose;
  Vec3f boidCenterMass;
};

class MyAgent : public PositionedVoice {
public:
  /* gam::Osc<> mAM;
  gam::ADSR<> mAMEnv;
  gam::Sine<> mOsc;
  gam::ADSR<> mAmpEnv;
  gam::EnvFollow<> mEnvFollow;
  gam::Pan<> mPan; */
  gam::Sine<> mSource;   // Sine wave oscillator source
  gam::Saw<> mModulator; // Saw wave modulator
  gam::AD<> mEnvelope;

  unsigned int mLifeSpan; // life span counter
  float mModulatorValue;  // To share modulator value from audio to graphics

  unsigned int mLifeSpan; // life span counter
  float mAMValue;
  int amFunc = 0;
  vector<int> i_boids;
  Vec3d myPos;
  MyAgent() {
    mEnvelope.lengths(5.0f, 5.0f);
    mEnvelope.levels(0, 1, 0);
    mEnvelope.sustainPoint(1);
    mModulator.freq(1.9);
    /* mAmpEnv.levels(0,1,1,0);
    mAmpEnv.lengths(5.0f, 5.0f);
    mAMEnv.curve(0);
    mAMEnv.levels(0,1,1,0);
    mAMEnv.lengths(5.0f, 5.0f); */
  }

  void onProcess(AudioIOData &io) override {
    while (io()) {
      mModulatorValue = mModulator();
      io.out(0) +=
          mEnvelope() * mSource() * mModulatorValue * 0.05; // compute sample
    }

    if (mEnvelope.done()) {
      free();
    }
    /* if (!isnan(mOsc.freq())){
      mAmpEnv.lengths()[0] = rand_double(0.1, 0.03);
      mAmpEnv.lengths()[2] = rand_double(1.7, 0.05);
      mPan.pos(-1.0);
      float amp = 0.015;//rand_float(0.015, 0.01);
      float amRatio = rand_float(2.0, 0.05);
      while (io()) {
        mAMValue = mAM();
        mAM.freq(mOsc.freq()*amRatio);            // set AM freq according to ratio
        float amAmt = mAMEnv();                    // AM amount envelope

        float s1 = mOsc();                        // non-modulated signal
        s1 = s1*(1-amAmt) + (s1*mAM())*amAmt;    // mix modulated and non-modulated

        s1 *= mAmpEnv() *amp;

        float s2 = mOsc() * mAmpEnv() * amp;
        float s3;
        mEnvFollow(s1);
        mPan(s1, s2, s3);
        io.out(0) += s1;
        io.out(1) += s2;
        io.out(3) += s3;
      }
      if (mAmpEnv.done()){//    change this too
        free();
      }
    } */
  }

  void onProcess(Graphics &g) override {
    // Get shared Mesh
    Mesh *sharedMesh = static_cast<Mesh *>(userData());
    mLifeSpan--;
    if (mLifeSpan == 0) { // If it's time to die, start die off
      mEnvelope.release();
    }
    g.pushMatrix();
    gl::polygonMode(GL_LINE);
    g.color(0.1, 0.9, 0.3);
    g.scale(mSize * mEnvelope.value() + mModulatorValue * 0.1);
    g.draw(*sharedMesh); // Draw the mesh
    g.popMatrix();
  }

  void set(float x, float y, float z, float size, float frequency,
           float lifeSpanFrames) {
    /* setPose(Pose({x, y, z}));
    mSize = size;
    mOsc.freq(frequency);
    mLifeSpan = lifeSpanFrames;
    myPos.x = x;
    myPos.y = y;
    myPos.z = z; */
    setPose(Pose({x, y, z}));
    mSize = size;
    mSource.freq(frequency);
    mLifeSpan = lifeSpanFrames;
    myPos.x = x;
    myPos.y = y;
    myPos.z = z;
  }

  // No get or set ParamFields functions as this system is not made to
  // be recordable.

  virtual void onTriggerOn() override {
    // We want to reset the envelope:
    /* mAMEnv.reset();
    mAmpEnv.reset();
    mAM.phase(-0.1);
    switch (amFunc) {
    case 0: mAM.source(tbSin); break;
    case 1: mAM.source(tbSqr); break;
    case 2: mAM.source(tbPls); break;
    case 3: mAM.source(tbDin); break;
    } */
    mEnvelope.reset();
    mModulator.phase(-0.1); // reset the phase
  }

  void updateFreq(vector<Boid>& boids){
    float avrg_freq = 0.0f;
    for (int i : this->i_boids) {
      avrg_freq += boids[i].frequency;
    }
    avrg_freq /= i_boids.size();
    if (avrg_freq <= 10){
      mOsc.freq(0);
    }else{
      mOsc.freq(avrg_freq);
    }
  }

  void updatePos(float x, float y, float z){
    setPose(Pose({x, y, z}));
  }

  Vec3d &getPos(){
    return myPos;
  }

  // No need for onTriggerOff() function as duration of agent's life is fixed
};

struct MyApp : DistributedAppWithState<CommonState> {
  
  Parameter timeStep{"Time Step", "", 2.0, "", 0.0333, 3.0};
  Parameter pointSize{"pointSize", "", 0.5, 0.05, 6.0};
  Parameter DBbRadius{"Dark Blue Boid Vision Radius", "", 0.7, 0.05, 1.5};
  Parameter DBcohesionForce{"Dark Blue Cohesion Force", "", 0.625, 0.0, 1.0};
  Parameter DBseparationForce{"Dark Blue Separation Force", "", 0.65, 0.0, 1.0};
  Parameter DBalignmentForce{"Dark Blue Alignment Force", "", 0.8, 0.0, 1.0};
  Parameter DBturnRate{"Dark Blue Turn Rate", "", 0.3, 0.0001, 1.0};

  std::vector<MyAgent> voices;
  std::vector<Boid> boids;    
  std::vector<Vec3f> food;
  vector<Vec3f> velocity;
  vector<Vec3f> force;
  vector<float> mass;
  Octree* boidTree{nullptr};

  double time{0};
  double foodRefresh{0};
  double boidRespawn{0};
  double timeScale{1.0};
  double angle{0};

  double initDist;

  Axes axes;
  Mesh DBMesh;
  Mesh voiceMesh;

  rnd::Random<> randomGenerator;

  // Nav point;
  DynamicScene scene;
  ShaderProgram pointShader;

  void onCreate() override {
    pointShader.compile(slurp("../point-vertex.glsl"),
                        slurp("../point-fragment.glsl"),
                        slurp("../point-geometry.glsl"));


    // place the camera so that we can see the axes
    initDist = al::dist(nav().pos(), Vec3d(0, 0, 0));
    nav().pos(Vec3f(5.0, 0.0, 0.0));
    nav().faceToward(Vec3d(0, 0, 0), Vec3d(0, 1, 0));

    // Don't do this:
    // nav().faceToward(0, 0, 0);
    // because it will be interpreted as this:
    // nav().faceToward(Vec3d(0), Vec3d(0), 0);
    // which has no effect because of the final 0!

    // Dark Blue Boid
    DBMesh.primitive(Mesh::TRIANGLE_FAN);
		DBMesh.vertex(0, 0, -3);        // Nose
		DBMesh.color(0, 0.5, 1.0);
		DBMesh.vertex(0, 1, 0);         // Top center edge ("back")
		DBMesh.color(0.45, 0.17, 0.28);
		DBMesh.vertex(-3, -1, 0);       // Left edge
		DBMesh.color(0, 0.15, 0.7);
		DBMesh.vertex(3, -1, 0);        // Right edge
		DBMesh.color(0.08, 0.08, 0.60);
		DBMesh.vertex(0, 1, 0);         // Top center edge, closing the fan
		DBMesh.color(0.45, 0.17, 0.28);

    setUp();
  } 
  
  void randomize(Nav& boidNav) {
    boidNav.pos(randomVec3f(CUBE_SIZE*0.95));
    boidNav.quat().set(r(), r(), r(), r()).normalize();
  }
  
  bool freeze = false;
  void onAnimate(double dt) override {
    // std::cout << nav().pos() << std::endl;
    if (isPrimary()) {
      if (freeze) return;
      dt *= timeStep.get();
      time += dt;
      boidTree->build(boids);

      Vec3d boidCenterOfMass(0, 0, 0);
      int i = 0;
      for (auto& b : boids) {
        double bRadius;
        double alignmentForce;
        double cohesionForce;
        double separationForce;
        double turnRate;
        if (b.getType() == "DB"){
          bRadius = DBbRadius.get();
          alignmentForce = DBalignmentForce.get();
          cohesionForce = DBcohesionForce.get();
          separationForce = DBseparationForce.get();
          turnRate = DBturnRate.get();
        }
        boidCenterOfMass += b.bNav.pos();
        b.handleBoundary(CUBE_SIZE);

        boidTree->queryRegion(b.bNav.pos(), Vec3f(bRadius), b.i_boids);

        b.boidForces(boids, alignmentForce, cohesionForce, separationForce, turnRate);
        b.updatePosition(dt);
        voices[b.voice_idx].updatePos(b.bNav.pos().x, b.bNav.pos().y, b.bNav.pos().z);

        state().boid[i].set(b.bNav);
        i++;
      }
      boidCenterOfMass /= boids.size();
      nav().faceToward(boidCenterOfMass, Vec3d(0, 1, 0), 0.2);
      state().pose = nav();
      
      for (int i = 0; i < boids.size(); i++) {
        for (int j = 0; j < NEIGHBOR_LIMIT; j++) {
          state().i_boids[i][j] = -1;
        }
        for (int j = 0; j < boids[i].get_i_boids_same_type(boids) && j < NEIGHBOR_LIMIT; j++) {
          if (boids[i].getType() == boids[boids[i].i_boids[j]].getType()){
            state().i_boids[i][j] = boids[i].i_boids[j];
          }
        }
      }

    } else {
      nav().set(state().pose);

      int i = 0;
      for (auto& b : boids) {
        b.bNav.set(state().boid[i]);
        i++;
      }

      for (int i = 0; i < boids.size(); i++) {
        boids[i].i_boids.clear();
        for (int j = 0; j < NEIGHBOR_LIMIT; j++) {
          int n = state().i_boids[i][j];
          if (n == -1) break;
          if (boids[n].getType() == boids[i].getType()){
            boids[i].i_boids.push_back(n);
          }
        }
      }
    }
  }

  void createVoice(float xPos, float yPos, float zPos, float freq){
    MyAgent *voice = scene.getVoice<MyAgent>();
    float size = randomGenerator.uniform(0.8, 1.2);
    int lifespan =
        graphicsDomain()->fps() * randomGenerator.uniform(6.0, 10.0);
    voice->set(xPos, yPos, zPos, // Place it in front
              size, freq, lifespan);
    scene.triggerOn(voice);
    voices.push_back(*voice);
  }

  bool onKeyDown(Keyboard const& k) override {
    switch (k.key()) {
      case ' ':
        // reset the simulation
        setUp();
        break;
    }
    return true;
  }

  void setUp() {  
      float delta_freq = 50.0f;       
      boidTree = new Octree(Vec3f(0, 0, 0), Vec3f(CUBE_SIZE), 0.01f);

      boids.clear();
      voices.clear();
      for (int i = 0; i < MAX_BOIDS; ++i) {
        Boid b;
        b.setType("DB");
        b.speed_percentage = 0.75f;
        float freq = 343.0f;
        b.frequency = rand_float(freq, delta_freq);
        randomize(b.bNav);
        createVoice(b.bNav.pos().x, b.bNav.pos().y, b.bNav.pos().z, b.frequency);
        b.voice_idx = voices.size() - 1;
        state().boid[i] = b.bNav.pos();
        boids.push_back(b);
      }
      
  }

  void onDraw(Graphics& g) override {
    // graphics / drawing settings
    g.clear(0);
    g.meshColor();
    g.pointSize(10);
    //axes.draw(g);

    int i = 0;
    for (auto& b : boids) {
      {
        Nav& a(b.bNav);
        g.pushMatrix();
        g.translate(a.pos());
        g.rotate(a.quat());
        g.scale(
          (i % 11 != 0) ? 0.03 : 0.02
        );
        if (b.getType() == "DB"){
          g.draw(DBMesh);
        }
        g.popMatrix();
      }
      ++i;
    }

    g.shader(pointShader);
    g.shader().uniform("pointSize", state().pointSize / 100);
    g.blending(true);
    g.blendTrans();
    g.depthTesting(true);
    scene.listenerPose(nav());
  }

  void onSound(AudioIOData &io) override{
    scene.render(io);
  }

  void onInit() override {
    auto cuttleboneDomain =
        CuttleboneStateSimulationDomain<CommonState>::enableCuttlebone(this);
    
    if (!cuttleboneDomain) {
      std::cerr << "ERROR: Could not start Cuttlebone. Quitting." << std::endl;
      quit();
    }

    if (isPrimary()) {
      auto guiDomain = GUIDomain::enableGUI(defaultWindowDomain());
      auto &gui = guiDomain->newGUI();
      auto speakers = AlloSphereSpeakerLayout();//StereoSpeakerLayout();
      scene.setSpatializer<Lbap>(speakers);
      addDodecahedron(voiceMesh);
      scene.setDefaultUserData(&voiceMesh);
      gam::addWave(tbSin, gam::SINE);
      gam::addWave(tbSqr, gam::SQUARE);      //IMPORTANTTTT
      gam::addWave(tbPls, gam::IMPULSE, 4);
      {    float A[] = {1, 0.7, 0.45, 0.3, 0.15, 0.08};
        float C[] = {10, 27, 54, 81, 108, 135};
        addSines(tbDin, A,C,6);
      }

      // Prepare the scene buffers according to audioIO buffers
      scene.prepare(audioIO());
      gui.add(timeStep);
      gui.add(pointSize);
      gui.add(DBcohesionForce);
      gui.add(DBseparationForce);
      gui.add(DBalignmentForce);
      gui.add(DBturnRate);
      gui.add(DBbRadius);
    }
  }
};

int main() {
  MyApp app;
  //app.configureAudio(48000, 512, 2, 0);
  //gam::sampleRate(app.audioIO().framesPerSecond());
  app.start();
}

string slurp(string fileName) {
  fstream file(fileName);
  string returnValue = "";
  while (file.good()) {
    string line;
    getline(file, line);
    returnValue += line + "\n";
  }
  return returnValue;
}
