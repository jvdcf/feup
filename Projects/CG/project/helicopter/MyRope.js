import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MySealedCylinder } from "../cuboid/MySealedCylinder.js";

export class MyRope extends CGFobject {
  constructor(scene) {
    super(scene);
    this.rope = new MySealedCylinder(scene,8);
    this.lenght = 0.3;
    this.material = new CGFappearance(this.scene);
    this.material.setAmbient(110/255,110/255,110/255, 0.6);
    this.material.setDiffuse(90/255,90/255,90/255, 0.7);
    this.material.setSpecular(220/255,220/255,220/255, 0.3);
  }

  update(len) {
    this.lenght = Math.max(len, 0.3);
  }

  display() {
    this.scene.pushMatrix();
      this.material.apply();
      this.scene.translate(0,0,-this.lenght + 0.2);
      this.scene.scale(0.05 / 2,0.05 / 2, this.lenght);
      this.rope.display();
    this.scene.popMatrix();
  }
  
}
