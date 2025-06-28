import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MyWindow } from "./MyWindow.js";
import { MyDoor } from "./MyDoor.js";
import { MyDoorSign } from "./MyDoorSign.js";
import { MyHeliport } from "./MyHeliport.js";
import { MyCuboid } from "../cuboid/MyCuboid.js";

/**
 * MyBuildingModule: A building module that is used by MyBuilding.
 *
 * @constructor
 * @param scene - Reference to MyScene object
 * @param {integer} width - Width of the building module
 * @param {integer} depth - Depth of the building module
 * @param {integer} floors - Number of floors of the building module
 * @param {integer} windows - Number of windows per floor
 * @param {boolean} isCentral - Whether the building module will be placed in
 * the center of the building or not. If true, an extra floor with a door and a
 * door sign will be placed at floor level.
 * @param {array<integer>[3]} color - Color of the building module in RGB format
 */
export class MyBuildingModule extends CGFobject {
  constructor(scene, width, depth, floors, windows, isCentral, color) {
    super(scene);
    this.floorHeight = 25;
    this.floors = isCentral ? floors + 1 : floors;
    this.width = width;
    this.depth = depth;
    this.height = this.floors * this.floorHeight;
    this.windows = windows;
    this.isCentral = isCentral;
    this.color = color;

    this.door = new MyDoor(this.scene);
    this.doorSign = new MyDoorSign(this.scene);
    this.window = new MyWindow(this.scene);
    this.heliport = new MyHeliport(this.scene);
    this.cuboid = new MyCuboid(
      this.scene,
      this.width,
      this.depth,
      this.height,
      this.color
    );
  }

  display() {
    this.scene.pushMatrix();
    this.cuboid.display();
    this.addWindows();
    if (this.isCentral) {
      this.addDoor();
      this.addDoorSign();
      this.addHeliport();
    }
    this.scene.popMatrix();
  }

  addWindows() {
    for (let f = this.isCentral ? 1 : 0; f < this.floors; f++) {
      const yDelta = -(this.depth / 2) - 0.1;
      const zDelta = -(this.height / 2) + (f + 1/2) * this.floorHeight;

      for (
        let xDelta = -this.width / 2 + this.width / 2 / this.windows;
        xDelta < this.width / 2;
        xDelta += this.width / this.windows
      ) {
        this.scene.pushMatrix();
        this.scene.translate(xDelta, yDelta, zDelta);
        this.window.display();
        this.scene.popMatrix();
      }
    }
  }

  addDoor() {
    if (!this.isCentral) return;
    const yDelta = -(this.depth / 2) - 0.1;
    const zDelta = -(this.height / 2) + this.door.height / 2;

    this.scene.pushMatrix();
    this.scene.translate(0, yDelta, zDelta);
    this.door.display();
    this.scene.popMatrix();
  }

  addDoorSign() {
    if (!this.isCentral) return;
    const yDelta = -(this.depth / 2) - 0.1;
    const b = this.height;
    const d = this.door.height;
    const f = this.floorHeight;
    const zDelta = -(b / 2) + d + (f - d) / 2;

    this.scene.pushMatrix();
    this.scene.translate(0, yDelta, zDelta);
    this.doorSign.display();
    this.scene.popMatrix();
  }

  addHeliport() {
    if (!this.isCentral) return;
    const zDelta = this.height / 2 + 0.1;
    const scale =
      Math.min(
        this.width / this.heliport.width,
        this.depth / this.heliport.height
      ) * 0.8;

    this.scene.pushMatrix();
    this.scene.translate(0, 0, zDelta);
    this.scene.scale(scale, scale, 1);
    this.heliport.display();
    this.scene.popMatrix();
  }

  enableNormalViz() {
    this.cuboid.enableNormalViz();
  }

  disableNormalViz() {
    this.cuboid.disableNormalViz();
  }

  updateBuffers(width, depth, floors, windows, color) {
    this.floors = this.isCentral ? floors + 1 : floors;
    this.width = width;
    this.depth = depth;
    this.height = this.floors * this.floorHeight;
    this.windows = windows;
    this.isCentral = this.isCentral;
    this.color = color;

    this.cuboid.updateBuffers(this.width, this.depth, this.height, this.color);
  }

  update(t) {
    if (!this.isCentral) return;
    this.heliport.update(t);
  }
}
