import { CGFscene, CGFcamera, CGFaxis, CGFappearance } from "../lib/CGF.js";
import { MyBuilding } from "./building/MyBuilding.js";
import { MyForest } from "./forest/MyForest.js";
import { MyGrass } from "./grass/MyGrass.js";
import { MyPanorama } from "./panorama/MyPanorama.js";
import { MyHeli } from "./helicopter/MyHeli.js";
import { MyCircle } from "./2d/MyCircle.js";
import { MyLake } from "./lake/MyLake.js";

/**
 * MyScene
 * @constructor
 */
export class MyScene extends CGFscene {
  constructor() {
    super();
  }

  init(application) {
    super.init(application);

    this.initCameras();
    this.initLights();

    //Background color
    this.gl.clearColor(0, 0, 0, 1.0);

    this.gl.clearDepth(100.0);
    this.gl.enable(this.gl.DEPTH_TEST);
    this.gl.enable(this.gl.CULL_FACE);
    this.gl.depthFunc(this.gl.LEQUAL);

    // Enable transparent textures
    this.gl.enable(this.gl.BLEND);
    this.gl.blendFunc(this.gl.SRC_ALPHA, this.gl.ONE_MINUS_SRC_ALPHA);

    this.enableTextures(true);

    //Initialize scene objects
    this.axis = new CGFaxis(this, 20, 1);
    this.grass = new MyGrass(this);
    this.panorama = new MyPanorama(this, "images/panorama.jpg");
    this.building = new MyBuilding(this, 70, 40, 2, 3, [0.4, 0.4, 0.4]);
    this.forest = new MyForest(this, 10, 7);
    this.helicopter = new MyHeli(this, 200, 200, this.building.getHeight() + 5);
    this.heliShadow = new MyCircle(this, 10, 10);
    this.heliShadowMaterial = new CGFappearance(this);
    this.heliShadowMaterial.setAmbient(0.1, 0.1, 0.1, 1);
    this.heliShadowMaterial.setDiffuse(0.8, 0.8, 0.8, 1);
    this.heliShadowMaterial.setSpecular(0, 0, 0, 0);
    this.heliShadowMaterial.setShininess(10.0);
    this.lake = new MyLake(this);

    // Interface variables
    this.fpsLimit = 60;
    this.lastFrameCall = Date.now();
    this.fpsLog = false;
    this.displayAxis = false;
    this.displayNormals = false;
    this.displayPanorama = true;
    this.displayBuilding = true;
    this.displayForest = true;
    this.displayHelicopter = true;
    this.displayLake = true;
    this.logHelicopterState = false;
    this.logHelicopterAction = false;
    this.objects = [];  // Used for debugging purposes
    this.todo = "// TODO";

    this.setUpdatePeriod(1000 / this.fpsLimit);
  }

  initLights() {
    this.lights[0].setPosition(-200, 200, 200, 1);  // Sun from panorama

    for (let i = 0; i <= 0; i++) {
      this.lights[i].setDiffuse(1, 1, 1, 1);
      this.lights[i].setSpecular(1, 1, 1, 1);
      this.lights[i].enable();
      this.lights[i].update();
    }
  }

  initCameras() {
    this.camera = new CGFcamera(
      0.6,
      0.1,
      2000,
      vec3.fromValues(400, 50, 400),
      vec3.fromValues(0, 0, 0)
    );
  }

  checkKeys(t) {
    if (this.gui.isKeyPressed("KeyW")) {
      if (this.logHelicopterAction) {
        console.log("W: Accelerate");
      }
      this.helicopter.accelerate(t);
    }
    if (this.gui.isKeyPressed("KeyS")) {
      if (this.logHelicopterAction) {
        console.log("S: Brake");
      }
      this.helicopter.brake(t);
    }
    if (this.gui.isKeyPressed("KeyA")) {
      if (this.logHelicopterAction) {
        console.log("A: Turn left");
      }
      this.helicopter.turnLeft(t);
    }
    if (this.gui.isKeyPressed("KeyD")) {
      if (this.logHelicopterAction) {
        console.log("D: Turn right");
      }
      this.helicopter.turnRight(t);
    }
    if (this.gui.isKeyPressed("KeyR")) {
      if (this.logHelicopterAction) {
        console.log("R: Reset helicopter");
      }
      this.helicopter.reset();
    }
    if (this.gui.isKeyPressed("KeyO")) {
      if (!this.helicopter.bucket.filled) return;
      if (this.helicopter.isLanded()) return;
      const forestWidth = this.forest.cellSize * this.forest.width;
      const forestHeight = this.forest.cellSize * this.forest.height;
      const one = this.helicopter.position.x > -250;
      const two = this.helicopter.position.x < (-250 + forestWidth);
      const three = this.helicopter.position.y > -250;
      const four = this.helicopter.position.y < (-250 + forestHeight);
      if (!(one && two && three && four)) return;

      if (this.logHelicopterAction) {
        console.log("O: Pour water");
      }

      this.helicopter.drop_water();
      this.forest.snubFire(this.helicopter.position.x + 250, this.helicopter.position.y + 250);
    }
    if (this.gui.isKeyPressed("KeyP")) {
      if (this.logHelicopterAction) {
        console.log("P: Start and raise helicopter");
      }
      this.helicopter.start_and_raise();
      this.building.centerModule.heliport.startUpAnim();
    }
    if (this.gui.isKeyPressed("KeyL")) {
      if (this.logHelicopterAction) {
        console.log("L: Stop and lower helicopter");
      }

      const one = this.helicopter.position.x > (-this.lake.width / 2 - 100);
      const two = this.helicopter.position.x < (this.lake.width / 2 - 100);
      const three = this.helicopter.position.y > (-this.lake.height / 2 + 250);
      const four = this.helicopter.position.y < (this.lake.height / 2 + 250);
      if (one && two && three && four) {
        this.helicopter.get_water(-100,250);
      } else {
        this.helicopter.stop_and_lower();
      }
      this.building.centerModule.heliport.startDownAnim();
    }
  }

  update(t) {
    this.checkKeys(t);
    this.helicopter.update(t);
    this.forest.update(t);
    this.building.update(t);
    if (!this.helicopter.isBeingAnimated()) {
      this.building.centerModule.heliport.stopAnim();
    }
    if (this.logHelicopterState) {
      console.log("State: ", this.helicopter.state);
      console.log("Position: ", this.helicopter.position.x, this.helicopter.position.y, this.helicopter.position.z);
      console.log("Orientation: ", this.helicopter.orientation);
    }
  }

  setDefaultAppearance() {
    this.setAmbient(0.5, 0.5, 0.5, 1.0);
    this.setDiffuse(0.5, 0.5, 0.5, 1.0);
    this.setSpecular(0.5, 0.5, 0.5, 1.0);
    this.setShininess(10.0);
  }

  async display() {
    // FPS Limit implementation
    const fpsInterval = 1000 / this.fpsLimit; // in milliseconds
    const endTime = Date.now();
    let elapsed = endTime - this.lastFrameCall;
    if (elapsed > 0) await new Promise(r => setTimeout(r, fpsInterval - elapsed));
    if (this.fpsLog) console.log("FPS: ", Math.min(parseInt(1000 / elapsed), this.fpsLimit));
    this.lastFrameCall = Date.now();

    // ---- BEGIN Background, camera and axis setup
    // Clear image and depth buffer everytime we update the scene
    this.gl.viewport(0, 0, this.gl.canvas.width, this.gl.canvas.height);

    this.gl.clear(this.gl.COLOR_BUFFER_BIT | this.gl.DEPTH_BUFFER_BIT);
    // Initialize Model-View matrix as identity (no transformation
    this.updateProjectionMatrix();
    this.loadIdentity();
    // Apply transformations corresponding to the camera position relative to the origin
    this.applyViewMatrix();

    this.lights[0].update();

    // Draw axis
    if (this.displayAxis) {
      this.axis.display();
    }

    if (this.displayNormals) {
      for (let i = 0; i < this.objects.length; i++) {
        this.objects[i].enableNormalViz();
      }
    } else {
      for (let i = 0; i < this.objects.length; i++) {
        this.objects[i].disableNormalViz();
      }
    }

    this.setDefaultAppearance();
    this.rotate(-Math.PI / 2, 1, 0, 0);

    this.grass.display();

    if (this.displayPanorama) this.panorama.display();

    if (this.displayBuilding) {
      this.pushMatrix();
      this.translate(200, 200, 0);
      this.rotate(-Math.PI / 4, 0, 0, 1);
      this.building.display();
      this.popMatrix();
    }

    if (this.displayForest) {
      this.pushMatrix();
      this.translate(-250, -250, 0);
      this.forest.display();
      this.popMatrix();
    }

    if (this.displayHelicopter) {
      this.helicopter.display();
      this.pushMatrix();
      this.heliShadowMaterial.apply();
      this.translate(this.helicopter.position.x, this.helicopter.position.y, 0.3);
      this.heliShadow.display();
      this.popMatrix();
    }

    if (this.displayLake) {
      this.pushMatrix();
      this.translate(-100, 250, 0.15); 
      this.lake.display();
      this.popMatrix();
    }
  }
}
