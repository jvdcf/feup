import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MyUnitCube } from "../cuboid/MyUnitCube.js";
import { MyRotor } from "./MyRotor.js"; 
import { MyStabilizer } from "./MyStabilizer.js";
import { MySealedCylinder } from "../cuboid/MySealedCylinder.js"; 

export class MyTail extends CGFobject {
  constructor (scene, diameter) {
    super(scene);
    this.scene = scene;
    this.diameter = diameter;
    this.stab = new MyStabilizer(scene);
    this.rotor = new MyRotor(scene, 0.1, diameter, 0.1);
    this.tail_body = new MySealedCylinder(scene, 12);    
    this.rotating = false;

    this.redMaterial = new CGFappearance(this.scene);
    let [r,g,b] = [220, 20, 20].map(x => x / 255);
    this.redMaterial.setAmbient(r,g,b, 0.5);
    this.redMaterial.setDiffuse(r,g,b, 0.7);
    this.redMaterial.setSpecular(r,g,b, 0.8);
    this.redMaterial.setShininess(20.);
  }

  update(t) {
    this.rotor.rotating = this.rotating;
    this.rotor.update(t);
  }

  display() {
    this.scene.pushMatrix();
      this.scene.pushMatrix();
        this.scene.rotate(Math.PI, 1, 0, 0);
        this.stab.display();
      this.scene.popMatrix();

      this.scene.pushMatrix();
        this.scene.translate(-0.3,0.15,-0.2);
        this.scene.rotate(-Math.PI / 2, 1, 0, 0);
        this.rotor.display();
      this.scene.popMatrix();
      
      this.scene.pushMatrix();
        this.scene.translate(-0.5,0,0.2);
        this.scene.scale(2., 0.05, 0.05);
        this.scene.rotate(Math.PI / 2, 0, 1, 0);
        this.redMaterial.apply();
        this.tail_body.display();
      this.scene.popMatrix();
      
    this.scene.popMatrix();
  }
  
}
