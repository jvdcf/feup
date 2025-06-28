import { CGFinterface, dat } from "../lib/CGF.js";

/**
 * MyInterface
 * @constructor
 */
export class MyInterface extends CGFinterface {
  constructor() {
    super();
  }

  init(application) {
    // call CGFinterface init
    super.init(application);

    // init GUI. For more information on the methods, check:
    // https://github.com/dataarts/dat.gui/blob/master/API.md
    this.gui = new dat.GUI();

    let f0 = this.gui.addFolder("Debug");
    f0.add(this.scene, "fpsLimit").name("FPS Limit").onChange((value) => {
      this.scene.setUpdatePeriod(value / 1000);
    });
    f0.add(this.scene, "fpsLog").name("Log FPS");
    f0.add(this.scene, "displayAxis").name("Display Axis");
    f0.add(this.scene, "displayNormals").name("Display Normals");

    let f1 = this.gui.addFolder("Sky-sphere");
    f1.add(this.scene, "displayPanorama").name("Display");
    f1.add(this.scene.panorama, "slices", 3, 64)
      .step(1)
      .name("Slices")
      .onChange((value) => {
        this.scene.panorama.updateBuffers(
          value + 1,
          this.scene.panorama.stacks
        );
      });
    f1.add(this.scene.panorama, "stacks", 6, 128)
      .step(1)
      .name("Stacks")
      .onChange((value) => {
        this.scene.panorama.updateBuffers(
          this.scene.panorama.slices,
          value + 1
        );
      });
    f1.add(this.scene.panorama, "radius", 0, 1000).step(1).name("Radius");

    let f2 = this.gui.addFolder("Building");
    f2.add(this.scene, "displayBuilding").name("Display");
    f2.add(this.scene.building, "width", 0, 100)
      .step(1)
      .name("Width")
      .onChange((value) => {
        this.scene.building.updateBuffers({ width: value });
      });
    f2.add(this.scene.building, "floors", 1, 10)
      .step(1)
      .name("Floors")
      .onChange((value) => {
        this.scene.building.updateBuffers({ floors: value });
        this.scene.helicopter.start_z = this.scene.building.getHeight() + 5;  
        if (this.scene.helicopter.isLanded()) {
          this.scene.helicopter.position.z = this.scene.building.getHeight() + 5;
        }
      });
    f2.add(this.scene.building, "windows", 1, 10)
      .step(1)
      .name("Windows per floor")
      .onChange((value) => {
        this.scene.building.updateBuffers({ windows: value });
      });
    /* f2.addColor(this.scene.building, "color")
      .name("Color")
      .onChange((value) => {
        value = value.map((c) => c / 256);
        this.scene.building.updateBuffers({ color: value });
      }); */

    let f3 = this.gui.addFolder("Forest and Fire");
    f3.add(this.scene, "displayForest").name("Display");
    f3.add(this.scene.forest, "width", 1, 30)
      .step(1)
      .name("Width")
      .onChange((value) => {
        this.scene.forest.updateBuffers(
          value,
          this.scene.forest.height,
          this.scene.forest.fireProb
        );
      });
    f3.add(this.scene.forest, "height", 1, 30)
      .step(1)
      .name("Height")
      .onChange((value) => {
        this.scene.forest.updateBuffers(
          this.scene.forest.width,
          value,
          this.scene.forest.fireProb
        );
      });
    f3.add(this.scene.forest, "fireProb", 0, 1)
      .step(0.01)
      .name("Fire Probability")
      .onChange((value) => {
        this.scene.forest.updateBuffers(
          this.scene.forest.width,
          this.scene.forest.height,
          value
        );
      });

    let f4 = this.gui.addFolder("Helicopter");
    f4.add(this.scene, "displayHelicopter").name("Display");
    f4.add(this.scene.helicopter, "speedFactor", 0.1, 3).step(0.1).name("Speed Factor");
    f4.add(this.scene, "logHelicopterState").name("Log State");
    f4.add(this.scene, "logHelicopterAction").name("Log Buttons");

    let f5 = this.gui.addFolder("Lake");
    f5.add(this.scene, "displayLake").name("Display");
    f5.add(this.scene.lake, "width", 100, 300).step(1).name("Width");
    f5.add(this.scene.lake, "height", 10, 100).step(1).name("Height");

    let f6 = this.gui.addFolder("Shaders");
    f6.add(this.scene, "todo").name(""); // TODO

    this.initKeys();

    return true;
  }

  initKeys() {
    // create reference from the scene to the GUI
    this.scene.gui = this;

    // disable the processKeyboard function
    this.processKeyboard = function () {};

    // create a named array to store which keys are being pressed
    this.activeKeys = {
      "KeyW": false, 
      "KeyS": false,
      "KeyA": false,
      "KeyD": false,
      "KeyR": false,
      "KeyP": false,
      "KeyL": false
    };
  }
  processKeyDown(event) {
    // called when a key is pressed down
    // mark it as active in the array
    this.activeKeys[event.code] = true;
  }

  processKeyUp(event) {
    // called when a key is released, mark it as inactive in the array
    this.activeKeys[event.code] = false;
  }

  isKeyPressed(keyCode) {
    // returns true if a key is marked as pressed, false otherwise
    return this.activeKeys[keyCode] || false;
  }
}
