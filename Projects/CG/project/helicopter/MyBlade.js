import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MyCuboid } from "../cuboid/MyCuboid.js";

export class MyBlade extends CGFobject {
  constructor(scene, diameter, thickness) {
    super(scene);
    this.color = [0x78, 0x78, 0x78].map((x) => x / 0xFF); 
    this.blade_body = new MyCuboid(scene, diameter, thickness, 0.01, this.color) // TODO: thin rectangle
  }

  display() {
    this.scene.pushMatrix();
    this.blade_body.display();
    this.scene.popMatrix();
  }
}
