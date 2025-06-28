import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MyBlade } from "./MyBlade.js";
import { MySealedCylinder } from "../cuboid/MySealedCylinder.js";

export class MyRotor extends CGFobject {
  constructor(scene, height, diameter, thickness) {
    super(scene);
    this.height = height;
    this.diameter = diameter;
    this.thickness = thickness;
    this.blade = new MyBlade(scene, diameter, thickness);
    this.cyl = new MySealedCylinder(scene, 12);    
    this.curr_angle = 0;
    this.prev_t = 0;
    this.t = 0;
    this.rotating = false;

    this.whiteMaterial = new CGFappearance(this.scene);
    let [r,g,b] = [220, 220, 220].map(x => x / 255);
    this.whiteMaterial.setAmbient(r,g,b, 0.5);
    this.whiteMaterial.setDiffuse(r,g,b, 0.7);
    this.whiteMaterial.setSpecular(r,g,b, 0.8);
    this.whiteMaterial.setShininess(20.);
  }

  update(t) {
    const delta_t = t - this.prev_t;
    this.prev_t = t;
    if (this.rotating) {
      this.t += delta_t;
      const speed_attenuation = 1000 / 3;
      this.curr_angle = 4 * Math.atan((this.t / speed_attenuation) % 1);
    }    
  }

  display() {
    this.scene.pushMatrix();       
      this.scene.pushMatrix();
        this.scene.translate(0, 0, -this.height);
        this.scene.scale(this.diameter / 70, this.diameter / 70, this.height);
        this.whiteMaterial.apply();
        this.cyl.display();
      this.scene.popMatrix();

      this.scene.pushMatrix();
        this.scene.scale(this.diameter / 20, this.diameter / 20, this.height / 10.);
        this.cyl.display();
        this.whiteMaterial.apply();
      this.scene.popMatrix();
    
      this.scene.pushMatrix();
        this.scene.rotate(this.curr_angle, 0, 0, 1);
        this.blade.display();
      this.scene.popMatrix();

    this.scene.popMatrix();
  }
}
