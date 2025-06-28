import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MyBuildingModule } from "./MyBuildingModule.js";

/**
 * MyBuilding: A fire building made up of three connected modules, with windows,
 * a main door and a door sign.
 * 
 * @constructor
 * @param scene - Reference to MyScene object
 * @param {integer} width - Width of the building
 * @param {integer} depth - Depth of the building
 * @param {integer} floors - Number of floors of the building (central module
 * will have an extra floor with a door and a door sign)
 * @param {integer} windows - Number of windows per floor
 * @param {array<integer>[3]} color - Color of the building
 */
export class MyBuilding extends CGFobject {
  constructor(scene, width, depth, floors, windows, color) {
    super(scene);
    this.moduleRatio = 0.75;
    this.width = width;
    this.depth = depth;
    this.floors = floors;
    this.windows = windows;
    this.color = color.map(c => c * 5); // TODO: Fix color handling
    this.centerModule = new MyBuildingModule(
      scene,
      width,
      depth,
      floors,
      windows,
      true,
      this.color,
    )
    this.smallModule = new MyBuildingModule(
      scene,
      width * this.moduleRatio,
      depth * this.moduleRatio,
      floors,
      windows,
      false,
      this.color,
    )
  }
  
  display() {
    this.scene.pushMatrix();
    this.scene.translate(0, 0, this.centerModule.height / 2);

    this.scene.pushMatrix();
    this.centerModule.display();
    this.scene.popMatrix();

    const xDelta = this.centerModule.width / 2 + this.smallModule.width / 2;
    const yDelta = this.centerModule.depth / 2 - this.smallModule.depth / 2;
    const zDelta = (- this.centerModule.height / 2 + this.smallModule.height / 2);

    this.scene.pushMatrix();
    this.scene.translate(xDelta, yDelta, zDelta);
    this.smallModule.display();
    this.scene.popMatrix();

    this.scene.pushMatrix();
    this.scene.translate(-xDelta, yDelta, zDelta);
    this.smallModule.display();
    this.scene.popMatrix();

    this.scene.popMatrix();
  }

  getHeight() {
    return (this.floors + 1) * this.centerModule.floorHeight;
  }

  enableNormalViz() {
    this.centerModule.enableNormalViz();
    this.smallModule.enableNormalViz();
  }
  disableNormalViz() {
    this.centerModule.disableNormalViz();
    this.smallModule.disableNormalViz();
  }

  updateBuffers() {
    this.centerModule.updateBuffers(
      this.width * 1.0, 
      this.depth * 1.0, 
      this.floors, 
      this.windows, 
      this.color
    );
    this.smallModule.updateBuffers(
      this.width * this.moduleRatio,
      this.depth * this.moduleRatio,
      this.floors,
      this.windows,
      this.color,
    );
  }

  update(t) {
    this.centerModule.update(t);
  }
}
